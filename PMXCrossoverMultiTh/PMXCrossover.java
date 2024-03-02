import java.util.Random;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;
public class PMXCrossover
{
    List<Integer> parent1, parent2, offSpring1, offSpring2;
    boolean mutation;

    public PMXCrossover(List<Integer> parent1, List<Integer> parent2)
    {
        this.parent1 = parent1;
        this.parent2 = parent2;
        this.offSpring1 = new ArrayList<>(parent1);
        this.offSpring2 = new ArrayList<>(parent2);
        this.mutation = false;
    }

    public void mutate()
    {
        //do we apply mutation
        Random random = new Random();
        int myInt = (int) AppInfo.mutationProbability * 100;
        if(myInt == random.nextInt(101))
        {

            int tempPosition1 = random.nextInt(offSpring1.size());
            int tempPosition2 = random.nextInt(offSpring1.size());

            // Prevent self-swapping
            while (tempPosition1 == tempPosition2)
            {
                tempPosition2 = random.nextInt(offSpring1.size());
            }

            //mutation for first path
            int temp = offSpring1.get(tempPosition1);
            offSpring1.set(tempPosition1, offSpring1.get(tempPosition2));
            offSpring1.set(tempPosition2, temp);

            //mutation for second path
            int temp2 = offSpring2.get(tempPosition1);
            offSpring2.set(tempPosition1, offSpring2.get(tempPosition2));
            offSpring2.set(tempPosition2, temp2);
        }
    }
    public void crossover()
    {
        // Use ThreadLocalRandom for better random number generation
        int cuttingPoint1 = ThreadLocalRandom.current().nextInt(parent1.size());
        int cuttingPoint2 = ThreadLocalRandom.current().nextInt(parent1.size());

        // Ensure cuttingPoint1 is less than cuttingPoint2
        if (cuttingPoint1 > cuttingPoint2)
        {
            int temp = cuttingPoint1;
            cuttingPoint1 = cuttingPoint2;
            cuttingPoint2 = temp;
        }

        //print the cutting points
        //System.out.println("Cutting point 1: " + cuttingPoint1);
        //System.out.println("Cutting point 2: " + cuttingPoint2);

        // Copy the segment from parent2 to offSpring1 and from parent1 to offSpring2
        for (int i = cuttingPoint1; i <= cuttingPoint2; i++)
        {
            offSpring1.set(i, parent2.get(i));
            offSpring2.set(i, parent1.get(i));
        }

        // Replace duplicate values in offSpring1 and offSpring2
        for (int i = 0; i < parent1.size(); i++)
        {
            if (i < cuttingPoint1 || i > cuttingPoint2)
            {
                int value1 = parent1.get(i);
                int value2 = parent2.get(i);

                // Replace duplicate values in offSpring1
                if (!offSpring1.subList(cuttingPoint1, cuttingPoint2 + 1).contains(value2))
                {
                    int index = i;
                    while (offSpring1.get(index) != value1)
                    {
                        index = parent2.indexOf(value1);
                    }
                    offSpring1.set(index, value2);
                }

                // Replace duplicate values in offSpring2
                if (!offSpring2.subList(cuttingPoint1, cuttingPoint2 + 1).contains(value1))
                {
                    int index = i;
                    while (offSpring2.get(index) != value2)
                    {
                        index = parent1.indexOf(value2);
                    }
                    offSpring2.set(index, value1);
                }
            }
        }

        //Display offsprings
        //System.out.println("Offspring 1: " + this.offSpring1);
        //System.out.println("Offspring 2: " + this.offSpring2);
    }

    public static int calculateEfficiency(List<Integer> path)
    {
        int pathLength = 0;

        for (int i = 0; i < path.size(); i++)
        {
            //last element of a path
            if (i == path.size() - 1)
            {
                pathLength += AppInfo.matrix[path.get(i)][path.get(0)];
            }
            //other elements
            else
            {
                pathLength += AppInfo.matrix[path.get(i)][path.get(i + 1)];
            }
        }


        return pathLength;
    }


    public boolean offspringsMoreEfficient()
    {
        int offSpringsLength = 0, parentsLength = 0;

        //calculate length
        offSpringsLength = calculateEfficiency(offSpring1) + calculateEfficiency(offSpring2);
        parentsLength = calculateEfficiency(parent1) + calculateEfficiency(parent2);


        //replace parents
        if(offSpringsLength > parentsLength)
            return false;

        return true;
    }

    public List<Integer> getOffSpring1()
    {
        return offSpring1;
    }

    public List<Integer> getOffSpring2()
    {
        return offSpring2;
    }
}
