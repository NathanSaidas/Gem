#ifndef OL_UTILITIES_H
#define OL_UTILITIES_H

#include <iostream>
#include <string>
#include <vector>

//This utilities file provides conversion functions from string to int/short/float etc and back



namespace Gem
{

    /*
    *   Function: convertStringToInt
    *   Return Type: int
    *   Description: This function takes in a string parameter and
    *   converts it to a int, then returns the int.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    
    int convertStringToInt(const std::string & aString);
    inline int S2I(const std::string & aString)
    {
        return convertStringToInt(aString);
    }
    /*
    *   Function: convertStringToUInt
    *   Return Type: unsigned int
    *   Description: This function takes in a string parameter and
    *   converts it to a unsigned int, then returns the unsigned int.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    unsigned int convertStringToUInt(const std::string & aString);
    /*
    *   Function: convertStringToShort
    *   Return Type: short
    *   Description: This function takes in a string parameter and
    *   converts it to a short, then returns the short.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    short convertStringToShort(const std::string & aString);
    /*
    *   Function: convertStringToUShort
    *   Return Type: unsigned short
    *   Description: This function takes in a string parameter and
    *   converts it to a unsigned short, then returns the unsigned short.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    unsigned short convertStringToUShort(const std::string & aString);
    /*
    *   Function: convertStringToFloat
    *   Return Type: float
    *   Description: This function takes in a string parameter and
    *   converts it to a float, then returns the float.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    float convertStringToFloat(const std::string & aString);
    inline float S2F(const std::string & aString)
    {
        return convertStringToFloat(aString);
    }
    /*
    *   Function: convertStringToDouble
    *   Return Type: double
    *   Description: This function takes in a string parameter and
    *   converts it to a double, then returns the double.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    double convertStringToDouble(const std::string & aString);
    /*
    *   Function: convertIntToString
    *   Return Type: string
    *   Description: This function takes in a const int parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertIntToString(const int & aNumber);
    inline std::string I2S(const int & aNumber)
    {
        return convertIntToString(aNumber);
    }
    /*
    *   Function: convertUIntToString
    *   Return Type: string
    *   Description: This function takes in a const unsigned int parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertUIntToString(const unsigned int & aNumber);
    /*
    *   Function: convertShortToString
    *   Return Type: string
    *   Description: This function takes in a const short parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertShortToString(const short & aNumber);
    /*
    *   Function: convertUShortToString
    *   Return Type: string
    *   Description: This function takes in a const unsigned short parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertUShortToString(const unsigned short & aNumber);
    /*
    *   Function: convertFloatToString
    *   Return Type: string
    *   Description: This function takes in a const float parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertFloatToString(const float & aNumber);
    inline std::string F2S(const float & aNumber)
    {
        return convertFloatToString(aNumber);
    }
    /*
    *   Function: convertDoubleToString
    *   Return Type: string
    *   Description: This function takes in a const double parameter and
    *   converts it to a string, then returns the string.
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    std::string convertDoubleToString(const double & aNumber);


    /*
    *   Function: isNumberString
    *   Return Type: bool
    *   Description: This function takes in a string parameter and
    *   determines if the string a pure number string ie "12346" or
    *   if the string contains a letter making it a string of characters ie "41f5tqhey"
    *   Date Modified: 13/1/2014 by Nathan Hanlan
    */
    bool isNumberString(const std::string & aString);
    /*
    *   Function: genRandomNumber
    *   Return Type: float
    *   Description: Generates a random number between a minimum and maximum
    *   call onLookerInit at least once before. 
    *   Date Modified: 23/02/2014 by Nathan Hanlan
    */
    float genRandomNumber(int aMin, int aMax);
    float genRandomNumber(int aMin, int aMax, int aPrecision);
    /*
    *   Function: onLookerInit
    *   Return Type: void
    *   Description: setups up OnLooker utility functions
    *   call before using OnLooker utility functions
    *   Date Modified: 23/02/2014 by Nathan Hanlan
    */
    void onLookerInit();
    /*
    *   Function: loadCompleteFile
    *   Return Type: std::string
    *   Description: Loads a complete file in from start to finish
    *   and returns it in the form of a string 
    *   Date Modified: 23/02/2014 by Nathan Hanlan
    */
    std::string loadCompleteFile(const char* filename);

    int getIndexOf(int col, int row, int maxRows);

    //Short hand loop
    #define loop(X) for(int i = 0; i < X; i++)

    //determine if the value is between the lower and higher bounds. Inclusive
    inline bool inBounds(const float & aValue,const float & aLower,const float & aHigher)
    {
        if(aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }

    inline bool inBounds(const int & aValue,const int & aLower,const int & aHigher)
    {
        if(aValue >= aLower && aValue <= aHigher)
        {
            return true;
        }
        return false;
    }

    inline void output(std::string aMsg)
    {
        std::cout << aMsg.c_str() << std::endl;
    }
    inline void output(std::string aMsg, float aNum)
    {
        std::cout << aMsg.c_str() << "(" << aNum << ")" << std::endl;
    }
    inline void log(std::string aMsg)
    {
        std::cout << "[Debug] " << aMsg.c_str() << std::endl;
    }
}


#endif