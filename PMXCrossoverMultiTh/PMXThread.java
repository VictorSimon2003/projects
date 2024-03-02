import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadLocalRandom;

public class PMXThread extends java.lang.Thread
{
    private int threadPopulation;
    List<List<Integer>> pathPopulation;
    private static Semaphore mergerSemaphore = new Semaphore(0);

    public PMXThread(int populationSize)
    {
        this.threadPopulation = populationSize;
        pathPopulation =  new ArrayList<>();
        generatePopulation();
    }

    private List<Integer> generatePath()
    {
        List<Integer> path = new ArrayList<>();
        boolean pathFound = false;
        int noPathElements = 0;

        //generate random path
        while(noPathElements < AppInfo.matrixSize)
        {
            int randomNumber = ThreadLocalRandom.current().nextInt(0,AppInfo.matrixSize);
            if(!path.contains(randomNumber))
            {
                path.add(randomNumber);
                noPathElements++;
            }
        }
        return path;
    }

    public void generatePopulation()
    {
        int generatedPopulationNo = 0;
        while(generatedPopulationNo < threadPopulation)
        {
            this.pathPopulation.add(this.generatePath());
            generatedPopulationNo++;
        }
        for(int i = 0; i < pathPopulation.size(); i++)
        {
            System.out.println(pathPopulation.get(i) + " -- " + getName());

        }
    }
    synchronized private void updateShortestPath(int length)
    {
        AppInfo.shortesPath = length;
    }

    public void calculateShortestPath()
    {
        //find shortest path from the population
        for (int i = 0; i < pathPopulation.size(); i++)
        {
            int efficiency = PMXCrossover.calculateEfficiency(pathPopulation.get(i));
            if( efficiency <= AppInfo.shortesPath)
            {
                updateShortestPath (efficiency);
            }
        }
    }
    public static void aquireMerger() throws InterruptedException
    {
        mergerSemaphore.acquire();
    }

    public static void releaselMerger() {
        mergerSemaphore.release();
    }

    synchronized private void increaseIterationNo()
    {
        AppInfo.noIterations++;
    }

    @Override
    public void run()
    {
        System.out.println("Thread Running -- " + getName());
        releaselMerger();
        while(!AppInfo.killThreads)
        {
            try
            {
                aquireMerger();
            }
            catch (InterruptedException e)
            {
                throw new RuntimeException(e);
            }
            // Do the crossover
            for (int i = 0; i < pathPopulation.size() - 1; i++)
            {
                List<Integer> currentPath = pathPopulation.get(i);
                List<Integer> nextPath = pathPopulation.get(i + 1);

                PMXCrossover pmxCrossover = new PMXCrossover(currentPath, nextPath);
                pmxCrossover.crossover();
                pmxCrossover.mutate();

                //offsprings are better than parents
                if(pmxCrossover.offspringsMoreEfficient() == true)
                {
                    pathPopulation.set(i, pmxCrossover.getOffSpring1());
                    pathPopulation.set(i + 1, pmxCrossover.getOffSpring2());
                }
            }
            increaseIterationNo();
           releaselMerger();
        }
        calculateShortestPath();

    }

    public List<List<Integer>> getPathPopulation()
    {
        return pathPopulation;
    }

    public void setPathPopulation(List<List<Integer>> pathPopulation)
    {
        this.pathPopulation = pathPopulation;
    }



}
