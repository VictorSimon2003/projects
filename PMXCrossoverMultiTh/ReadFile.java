import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import static java.lang.Character.isWhitespace;

public class ReadFile
{
    File fptr;
    int [][]matrix;
    int size;
    Scanner myReader;
    public ReadFile(File fptr)
    {
        //set Scanner
        this.myReader = null;
        this.size = 0;
        try
        {
            this.myReader = new Scanner(fptr);

            if (myReader.hasNextLine())
            {
                // get matrix size from the first line, trimming any leading or trailing whitespaces
                this.size = Integer.parseInt(myReader.nextLine().trim());
            }
            else
            {
                throw new RuntimeException("File is empty");
            }
        }
        //File not found
        catch (FileNotFoundException e)
        {
            System.out.println("FILE not found!");
            throw new RuntimeException(e);
        }

        matrix = new int[size][size];
    }

    public void setMatrix()
    {
        if (size != 0) {
            int noPassedLines = 0;

            while (myReader.hasNextLine())
            {
                int[] line = new int[size];
                int noPassedDigits = 0;
                String charLine = myReader.nextLine();

                for (int i = 0; i < charLine.length(); i++)
                {
                    int currentNo = 0;

                    // Check if the character is a digit
                    if (Character.isDigit(charLine.charAt(i)))
                    {
                        // Parse the number
                        while (i < charLine.length() && Character.isDigit(charLine.charAt(i)))
                        {
                            currentNo = currentNo * 10 + Integer.parseInt(String.valueOf(charLine.charAt(i)));
                            i++;
                        }
                        line[noPassedDigits++] = currentNo;
                    }
                }

                matrix[noPassedLines++] = line.clone();
            }
        }
        else
        {
            System.out.println("Size of matrix is zero. Unable to set matrix.");
        }
    }


    public int[][] getMatrix()
    {
        try
        {
            return matrix;
        }
        catch (Exception e)
        {
            System.out.println("Cannot return matrix!");
            return null;
        }
    }

    public int getMatrixSize()
    {
        return size;
    }
    public void closeScanner()
    {
        if (myReader != null)
        {
            myReader.close();
        }
    }

}
