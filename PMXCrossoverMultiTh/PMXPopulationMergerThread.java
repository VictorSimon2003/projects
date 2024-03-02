import java.util.ArrayList;
import java.util.List;
import java.lang.Thread;
import java.util.concurrent.Semaphore;

public class PMXPopulationMergerThread extends Thread
{
    private List<List<Integer>> pathPopulation;
    private List<Integer> pathPopulationEfficiency;
    public PMXPopulationMergerThread()
    {
        pathPopulation = new ArrayList<>();
        pathPopulationEfficiency = new ArrayList<>() ;//int[AppInfo.totalNoPopulation];
    }

    private int getPathEfficiency(List<Integer> path)
    {
        return PMXCrossover.calculateEfficiency(path);
    }

    //make an arrayList of path efficiency
    private void calculatePathEfficiency()
    {
        pathPopulationEfficiency.clear();
        for(List<Integer> currentPath : pathPopulation)
        {
            pathPopulationEfficiency.add(getPathEfficiency(currentPath));
        }
    }
    private void mergePopulation()
    {
        pathPopulation.clear();
        System.out.println("Merging pop!");
        for(PMXThread currentPMXThread : AppInfo.PMXProcessingThreads)
        {
            this.pathPopulation.addAll(currentPMXThread.getPathPopulation());
        }
    }

    private void sortPathPopulation()
    {
        int temp, size = pathPopulationEfficiency.size();
        List<Integer> pathTemp = new ArrayList<>();
        boolean swapped;
        for (int i = 0; i < size - 1; i++)
        {
            swapped = false;
            for (int j = 0; j < size - i - 1; j++)
            {
                if (pathPopulationEfficiency.get(j) < pathPopulationEfficiency.get(j+1))
                {
                    // Swap pathPopulationEfficiency.get(j) and pathPopulationEfficiency.get(j+1)
                    temp = pathPopulationEfficiency.get(j);
                    pathPopulationEfficiency.set(j, pathPopulationEfficiency.get(j + 1));
                    pathPopulationEfficiency.set(j + 1, temp);
                    swapped = true;

                    //Swap pathPopulation.get(j) and pathPopulation.get(j+1)
                    pathTemp = pathPopulation.get(j);
                    pathPopulation.set(j, pathPopulation.get(j + 1));
                    pathPopulation.set(j + 1, pathTemp);

                }
            }

            // If no two elements were swapped by inner loop, then break
            if (swapped == false)
                break;
        }
    }

    public void distributeNewPathPopulation()
    {
        int i = 0, currentThreadNo = 0;
        for(PMXThread currentPMXThread : AppInfo.PMXProcessingThreads)
        {

            currentPMXThread.setPathPopulation(pathPopulation.subList(i, i + AppInfo.threadNoPopulation[currentThreadNo]));
            i += AppInfo.threadNoPopulation[currentThreadNo] - 1;
            currentThreadNo++;
        }
    }



    @Override
    public void run()
    {
        while(!AppInfo.killThreads)
        {

            if(AppInfo.mergePopulation == true)
            {
                System.out.println("PMXPopulationMergerThread runs!");;
                mergePopulation();
                calculatePathEfficiency();
                sortPathPopulation();
                distributeNewPathPopulation();
                AppInfo.mergePopulation = false;
            }
        }
    }


}
