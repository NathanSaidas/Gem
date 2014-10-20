#include "G_Utilities.h"


#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
namespace Gem
{
	int convertStringToInt(const std::string & aString)
    {
        int number = 0;
        std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0;
        }

        return number;
    }
	int convertStringToInt(const Gem::string & aString)
	{
		int number = 0;
		std::stringstream stream(aString.c_str());
		stream >> number;
		if (stream.fail())
		{

			return 0;
		}

		return number;
	}
	unsigned int convertStringToUInt(const std::string & aString)
    {
        unsigned int number = 0;

		std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0;
        }

        return number;
    }
	short convertStringToShort(const std::string & aString)
    {
        short number = 0;

		std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0;
        }

        return number;
    }
	unsigned short convertStringToUShort(const std::string & aString)
    {
        unsigned short number = 0;

		std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0;
        }

        return number;
    }
	float convertStringToFloat(const std::string & aString)
    {
        float number = 0.0f;

		std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0.0f;
        }

        return number;
    }
	float convertStringToFloat(const string & aString)
	{
		float number = 0.0f;

		std::stringstream stream(aString.c_str());
		stream >> number;
		if (stream.fail())
		{

			return 0.0f;
		}

		return number;
	}
	double convertStringToDouble(const std::string & aString)
    {
        double number = 0.0;

		std::stringstream stream(aString);
        stream >> number;
        if(stream.fail())
        {
            
            return 0;
        }

        return number;
    }

	std::string convertIntToSTDString(const int & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }
	string convertIntToString(const int & aNumber)
	{
		string returnString;

		std::stringstream stream;

		stream << aNumber;

		return stream.str().c_str();
	}
	std::string convertUIntToString(const unsigned int & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }
	std::string convertShortToString(const short & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }
	std::string convertUShortToString(const unsigned short & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }
	std::string convertFloatToSTDString(const float & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }
	string convertFloatToString(const float & aNumber)
	{
		string returnString;

		std::stringstream stream;

		stream << aNumber;

		return stream.str().c_str();
	}
	std::string convertDoubleToString(const double & aNumber)
    {
        string returnString;

		std::stringstream stream;

        stream << aNumber;

        return stream.str();
    }

    bool isNumberString(const std::string & aString)
    {
        //get a counting variable
        int counter = 0;
        //loop through all the elements of the string
        for (int i = 0; i < aString.size(); i++)
        {
            //if the element is a digit, increment the counter
            //else break out and return false.
            if (isdigit(aString[i]))
            {
                counter++;
            }
            else
            {
                break;
            }
        }
        if (counter == aString.size())
        {
            return true;
        }
        return false;

    }

    float genRandomNumber(int aMin, int aMax)
    {
        if(aMin > aMax)
        {
            int temp = aMin;
            aMin = aMax;
            aMax = temp;
        }
        int difference = (aMax+1) - aMin;
        return (float)(rand() % difference + aMin);
    }
    
    float genRandomNumber(int aMin, int aMax, int aPrecision)
    {
        if(aPrecision > 6)
        {
            aPrecision = 6;
        }
        else if(aPrecision < 0)
        {
            aPrecision = 0;
        }
        
        float precisionPower = pow(10.0f,aPrecision);
        float rand = genRandomNumber((int)(aMin * precisionPower), (int)(aMax * precisionPower));
        return (rand/precisionPower);
    }


    void onLookerInit()
    {
        srand((unsigned int)time(0));
    }
	std::string loadCompleteFile(const char* filename)
    {
		std::string contents;
		std::ifstream readStream(filename);
	    if(!readStream.is_open())
	    {
            
			std::ofstream  writeStream(filename);
            writeStream << "Heres the file";
            writeStream.close();

		    return contents;
	    }

	    char * buffer = new char [1000];
	    while(!readStream.eof())
	    {
		    readStream.getline(buffer,1000);
		    contents+=buffer;
		    contents+='\n';
	    }

        return contents;
    }

    int getIndexOf(int col, int row, int maxRows)
    {
        return (col + row * maxRows);
    }


}