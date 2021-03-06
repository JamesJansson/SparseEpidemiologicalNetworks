// Developed by James Jansson
#include <fstream>
#include <sstream>// may not be necesary in final build
#include "SimpleMatrix.h"
using namespace std;





class csvfile {

    vector <string> FileLines;
    vector <vector <string> > StringMatrix;
    vector <vector <int> > IntMatrix;
    vector <vector <float> > FloatMatrix;
    // int array to store the converted strings
    // float array to store the converted strings

public:
    vector<string> SplitLine(const std::string&  LineString);//note that this should eventually not be public (following testing)



    int Open(string FileName);//loads file and data
    int GetInt(int x, int y);
    int GetInt(int xStart, int xEnd, int yStart, int yEnd);
    float GetFloat(int x, int y);
    float GetFloat(int xStart, int xEnd, int yStart, int yEnd);
    int ConvertToInt(void);
    int ConvertToFloat(void);
    //int ReadInt(string FileName, int ReadDimensionXStart, int ReadDimensionXEnd, int ReadDimensionYStart, int ReadDimensionYEnd);
    //float ReadFloat(string FileName, int ReadDimensionXStart, int ReadDimensionXEnd, int ReadDimensionYStart, int ReadDimensionYEnd);
    //string ReadString(string FileName, int ReadDimensionXStart, int ReadDimensionXEnd, int ReadDimensionYStart, int ReadDimensionYEnd);
    int xSize(void);
    int ySize(void);

    SimpleMatrix<int> outputint(void);
    SimpleMatrix<int> outputint(int xStart, int xEnd, int yStart, int yEnd);

    void DisplayFile(void);

    // Writing functions
    int Add(SimpleMatrix<int> ArrayOf, int xpos, int ypos);// Converts content to a string



    // SaveFile
    // is the string contains a " or a , then the string has to be put in quotes ("_") and all quotes in the original string have to be turned into double quote marks


};


int csvfile::Open(string FileName)
{
    std::ifstream infile;
    string CurrentLine;
    infile.open(FileName, std::ifstream::in);
	std::string LineString;
    if (infile.is_open()==false)
    {
        std::cout << "\nError opening file";
        return -1;
    }

    FileLines.clear();
    // Read in the files
    while (infile.good())
    {
        getline(infile, CurrentLine, '\n');
        FileLines.push_back(CurrentLine);
    }
    infile.close();

    //Convert the file lines to individual strings
    for(string CurrentString : FileLines)
    {
        StringMatrix.push_back(SplitLine(CurrentString));//Split lines returns a vector of strings
    }
    ConvertToInt();
    ConvertToFloat();
    return 0;
}


vector<string> csvfile::SplitLine(const std::string&  LineString)
{
    bool EntryEndReached;
    bool QuoteOpen;

    string CurrentString;
    vector<string> StringPart;
    //Determine the length of the string
    int StringLength=LineString.length();
    int charpos=0;

    //for each of the entries
    int EntryCount=0;
    while (charpos<StringLength)
    {
        EntryCount++;
        CurrentString.clear();

        EntryEndReached=false;
        if (LineString[charpos]=='"')//is a quotemark
        {
            QuoteOpen=true;
            charpos++;
        }
        else
        {
            QuoteOpen=false;
        }

        //keep going while not the last element of the line
        while (EntryEndReached==false && charpos<StringLength)
        {
            if (QuoteOpen==false && LineString[charpos]==',')//if a character is found and is not being protect by quotes being open
            {
                EntryEndReached=true;
                charpos++;
            }
            else if (QuoteOpen==true && LineString[charpos]=='"')//is a quotemark
            {
                if (charpos+1==StringLength) //it is the last element of the string (to avoid accessing memory illegally)
                {
                    QuoteOpen=false; //it must be a quote close mark
                    charpos++;
                }
                else if (LineString[charpos+1]=='"')//the next character is also a quote mark
                {
                    // the quote is still open
                    // add a quote mark to the string
                    CurrentString.push_back('"');
                    charpos++;//jump twice because of the escape character
                    charpos++;//jump twice because of the escape character
                }
                else
                {
                    QuoteOpen=false; //it must be a quote close mark
                    charpos++;
                }
            }
            else
            {
                CurrentString.push_back(LineString[charpos]);
                charpos++;
            }
        }

        //note that an empty string should be counted as an entry, even if it is at the end of a line
        //Add a new entry
        StringPart.push_back(CurrentString);
    }
    //return a vector of strings, to be converted later
    return StringPart;
}

int csvfile::ConvertToInt(void)
{
    //Clear out the current int values
    IntMatrix.clear();
    vector<int> CurrentLineInt;
    int ValueToStore;
    for(vector<string> CurrentLine : StringMatrix )
    {
        CurrentLineInt.clear();
        for (string CurrentString : CurrentLine)
        {
            istringstream ( CurrentString ) >> ValueToStore;
            CurrentLineInt.push_back(ValueToStore);
        }
        IntMatrix.push_back(CurrentLineInt);
    }
    return 0;
}

int csvfile::ConvertToFloat(void)
{
    //Clear out the current int values
    FloatMatrix.clear();
    vector<float> CurrentLineFloat;
    float ValueToStore;
    for(vector<string> CurrentLine : StringMatrix )
    {
        CurrentLineFloat.clear();
        for (string CurrentString : CurrentLine)
        {
            istringstream ( CurrentString ) >> ValueToStore;
            CurrentLineFloat.push_back(ValueToStore);
        }
        FloatMatrix.push_back(CurrentLineFloat);
    }
    return 0;
}

void csvfile::DisplayFile(void)
{
    //this section is used primarily for testing
    cout<<"File lines"<<endl;
    for(string CurrentString : FileLines)
        cout << "  " << CurrentString << endl;
    cout<<"File split"<<endl;
    for(vector<string> CurrentLine : StringMatrix )
    {
        for (string CurrentString : CurrentLine)
        {
            cout<<CurrentString<<":";
        }
        cout<<endl;
    }

    cout<<"Int split"<<endl;
    for(vector<int> CurrentLine : IntMatrix )
    {
        for (int CurrentInt : CurrentLine)
        {
            cout<<CurrentInt<<":";
        }
        cout<<endl;
    }

    cout<<"Float split"<<endl;
    for(vector<float> CurrentLine : FloatMatrix )
    {
        for (float CurrentFloat : CurrentLine)
        {
            cout<<CurrentFloat<<":";
        }
        cout<<endl;
    }
}

int csvfile::GetInt(int x, int y)//not that the first element is x=0
{
    //determine if the y dimension is big enough
    if (y>=IntMatrix.size())
    {
        return 0;
    }
    else
    {
        vector<int>Row;
        Row=IntMatrix[y];
        if (x>=Row.size())
        {
            return 0;
        }
        else
        {
            return Row[x];
        }
    }
}

float csvfile::GetFloat(int x, int y)//not that the first element is x=0
{
    //determine if the y dimension is big enough
    if (y>=FloatMatrix.size())
    {
        return 0;
    }
    else
    {
        vector<float>Row;
        Row=FloatMatrix[y];
        if (x>=Row.size())
        {
            return 0;
        }
        else
        {
            return Row[x];
        }
    }
}

/*vector<vector<int>> csvfile::GetInt(int xStart, int xEnd, int yStart, int yEnd)
{
    vector<vector<int>> ReturnMatrix;
    SimpleMatrix<int> ReturnMatrix;
    for (ycount=yStart; ycount<yEnd; ycount++
    {
        for (xcount=xStart; xcount<xEnd; xcount++
        {

        }
    }
}*/

int csvfile::ySize(void)
{
    return StringMatrix.size(); //note that excel adds an empty line at the end of csv files
}

int csvfile::xSize(void)
{
    int LargestX=0;
    for(vector<string> CurrentLine : StringMatrix )
        LargestX=(CurrentLine.size()>LargestX)?CurrentLine.size():LargestX;
    return LargestX;
}






/*
#define csvreadmaxrows 30000
#define csvreadmaxcolumns 300

//int version
int csvread(std::string FileName, int** FileData, const int ReadDimensionXStart, const int ReadDimensionXEnd, const int ReadDimensionYStart, const int ReadDimensionYEnd)
{
	std::ifstream infile;
	std::istringstream FileLineStream;
	//Set maximum size of array
	int MaxArraySizeX=ReadDimensionXEnd-ReadDimensionXStart+1;
	int MaxArraySizeY=ReadDimensionYEnd-ReadDimensionYStart+1;
	//int *FileData=new int [MaxArraySizeX * MaxArraySizeY];//Note that this is a 1 dimensional declaration for a 2D array used as a hack to overcome memory issues in dynamic allocation
	//int** ary = new int*[sizeX];
	//	for(int i = 0; i < sizeX; ++i)
	//	ary[i] = new int[sizeY];

	int LineCount, CommaCount;
	int ColumnCount, RowCount;
	char Line[csvreadmaxrows] ;
	char CharNumber[csvreadmaxcolumns];

	std::cout<<"Insert into here a try catch exception to handle a lack of the appropriate file. "<<endl;
	std::cout<<"Also try to handle exceptions to the file dimension size maximum. "<<endl;

    infile.open (FileName, std::ifstream::in);
	std::string LineString;
    if (infile.is_open())
    {
		RowCount=0;
		LineCount=0;
        while (infile.good())
        {
			FileLineStream.clear();
			infile.getline(Line, csvreadmaxrows, '\n');
			if (LineCount>=ReadDimensionYStart && LineCount<=ReadDimensionYEnd)
			{
				LineString=Line;
				FileLineStream.str(LineString);
				ColumnCount=0;
				CommaCount=0;
				while (FileLineStream.good() && ColumnCount<=(ReadDimensionXEnd-ReadDimensionXStart))
				{
					FileLineStream.getline(CharNumber, csvreadmaxcolumns, ',');

					if (CommaCount>=ReadDimensionXStart && CommaCount<=ReadDimensionXEnd)
					{
						FileData[ColumnCount][RowCount]= atoi(CharNumber) ;
						ColumnCount++;
					}
					CommaCount++;
				}
				RowCount++;
			}
			LineCount++ ;

        }
		//Testing: testing to see that the data is being read in properly
		bool TestingOn=false;
		if (TestingOn)
		{
			for (int i=0; i<MaxArraySizeY; i++)
			{
				std::cout<<"\n"<<i<<':';
				for (int j=0; j<MaxArraySizeX; j++)
				{
					std::cout<<FileData[j][i]<<' ';
				}
			}
		}
        infile.close();
    }
    else
    {
        std::cout << "\nError opening file";
    }
	//std::cout << "\nWe may need to eliminate the delete line if the values need to be transfered back through the pointer";
	//delete[] FileData;
	return 0;
}

//float version
int csvread(std::string FileName, float** FileData, const int ReadDimensionXStart, const int ReadDimensionXEnd, const int ReadDimensionYStart, const int ReadDimensionYEnd)
{
	std::ifstream infile;
	std::istringstream FileLineStream;
	//Set maximum size of array
	int MaxArraySizeX=ReadDimensionXEnd-ReadDimensionXStart+1;
	int MaxArraySizeY=ReadDimensionYEnd-ReadDimensionYStart+1;

	int LineCount, CommaCount;
	int ColumnCount, RowCount;
	char Line[csvreadmaxrows] ;
	char CharNumber[csvreadmaxcolumns];
    infile.open (FileName, std::ifstream::in);
	std::string LineString;
    if (infile.is_open())
    {
		RowCount=0;
		LineCount=0;
        while (infile.good())
        {
			FileLineStream.clear();
			infile.getline(Line, csvreadmaxrows, '\n');
			if (LineCount>=ReadDimensionYStart && LineCount<=ReadDimensionYEnd)
			{
				LineString=Line;
				FileLineStream.str(LineString);
				ColumnCount=0;
				CommaCount=0;
				while (FileLineStream.good() && ColumnCount<=(ReadDimensionXEnd-ReadDimensionXStart))
				{
					FileLineStream.getline(CharNumber, csvreadmaxcolumns, ',');

					if (CommaCount>=ReadDimensionXStart && CommaCount<=ReadDimensionXEnd)
					{
						FileData[ColumnCount][RowCount]= atof(CharNumber) ;
						ColumnCount++;
					}
					CommaCount++;
				}
				RowCount++;
			}
			LineCount++ ;

        }
		//Testing: testing to see that the data is being read in properly
		bool TestingOn=false;
		if (TestingOn)
		{
			for (int i=0; i<MaxArraySizeY; i++)
			{
				std::cout<<"\n"<<i<<':';
				for (int j=0; j<MaxArraySizeX; j++)
				{
					std::cout<<FileData[j][i]<<' ';
				}
			}
		}
        infile.close();
    }
    else
    {
        std::cout << "\nError opening file";
    }
	return 0;
}

int csvwrite(std::string FileName, float** FileData, int DimensionX, int DimensionY)
{
	std::ofstream Output;
	Output.open (FileName);

	for (int i=0; i<DimensionY; i++)
	{
		for (int j=0; j<DimensionX; j++)
		{
			Output << FileData[j][i]<<",";
		}
		Output << "\n";
	}
	Output.close();
	return 0;
}

*/
