// Developed by James Jansson

// This library is essentially compiler candy, designed to reduce the size of matrix operations from several lines down to a single line.
// At this stage, all operations are done elementwise.
// Please note that none of these functions have been security tested as they are intended for scientific computing.
// This library will likely require c++11 setting turned on in your compiler.

#include <stdarg.h>

//Settings
//int SMThreadsToUse=1;//will be used for multithreading in a future life
//int SMMaxEntries=10000000;//used to prevent memory explosions


// Class declarations
template <typename TemplateType>
class SimpleMatrix {

    vector<TemplateType> ValueArray;//Data is stored in the value array
    int NDimSize; //Number of dimensions
    vector<int> DimSize; //size of each dimension
    int TotalArraySize;//equals the dimensions of the array multiplied together
    bool Extendable=false;//determines if adding values to areas outside the matrix is allowed. At the moment, only predefined sized Matrices will be allowed for simplicity. In future releases this may be allowed, but will require a cleaver method to extend teh vector size
    vector<int> Base;//used to reference cells in the matrix

    //Indexing
        vector<int> ThisIndex;//designed to be unchanging in size to prevent memory rewrites constantly
        int ThisIndexSize;//counts the
        int CreateIndexLinear(int i);
        int CreateIndex2D(int i, int j);
        template<typename... Args>
        bool ConvertIndexToVector(int t1, Args... args)
        {
            ThisIndexSize++;
            if (ThisIndexSize > NDimSize)//if it already exceed expected dimension size
                return 0;
            ThisIndex[ThisIndexSize-1]=t1;
            return ConvertIndexToVector(args...);//eventually will return true for correct number of index elements.
        }
        bool ConvertIndexToVector(int t1)
        {
            ThisIndexSize++;
            if (ThisIndexSize != NDimSize)//if it has got to the end and is not exactly the right size (either too big or too small
                return 0;
            ThisIndex[ThisIndexSize-1]=t1;
            return 1;//indicate that it worked
        }

        //This is a recursive argument counter, as described here: http://stackoverflow.com/a/16338804/1275985
        vector<int> ConstructorArgStorage;
        int NumConstructorArgs;
        int CountConstructorArgs(int t)
        {
            ConstructorArgStorage.push_back(t);
            NumConstructorArgs++;
            return NumConstructorArgs;
        }
        template<typename... Args>
        int CountConstructorArgs(int t, Args... args)
        {
            ConstructorArgStorage.push_back(t);
            NumConstructorArgs++;
            return CountConstructorArgs(args...);
        }

        //Look up functions designed to references to the correct memory locations
        bool InRange(vector<int> Index);//used privately to determine that the index is in the range sepcified
        int IndexPos(vector<int> Index);//used privately to determine the linear array
        int IndexPosCheck(vector<int> Index);


    public:
    //Constructors
        template <typename... ArgType>
        SimpleMatrix(int FirstIndex, ArgType... args);//constructor should force all in specified matrices pass through this function while not diverting other defintitions such as vector<int>
        SimpleMatrix(vector<int> TempDimSize);//alows a vector to be used to specify the dimensions of the matrix
        //SimpleMatrix(const SimpleMatrix<int>& TempDimSize);
        SimpleMatrix(void);//allows the quick defintion of a single value to be represented as a SimpleMatrix type to making pointer functions a whole heap easier
        void CreateValueArray(void);//sets up memory to do operations on

    vector<int> Dim(void);
        int NDim(void);//a simple integer value of the number of dimensions
        int TotalElements(void);
        //Checking and error functions
        template <typename OtherType>
        bool DimensionsCompatible( SimpleMatrix<OtherType> Other);
        template <typename OtherType>
        void StopIfDimensionsIncompatible( SimpleMatrix<OtherType> Other);

    //Setting functions
//    void Set( TemplateType SetValue, vector<int> Index);//this version should represent an array of indices
//    void Set(const SimpleMatrix<int>& Index, TemplateType SetValue);
//    template <typename... ArgType>
//    void Set( TemplateType SetValue, int FirstIndex, int SecondIndex, ArgType... args);//allows a vector to be set for easy use of the matrix
//    void Set( TemplateType SetValue, int LinerIndex);//allows a vector to be set for easy use of the matrix
        void SetLinearIndex( TemplateType SetValue, int LinerIndex);//allows a vector to be set for easy use of the matrix
//    void Set(bool CheckDimension,  TemplateType SetValue, vector<int> Index);//to allow faster execution without checking

    //Operations on the matrix
        template <typename CopyFromType>
        void Set(SimpleMatrix<CopyFromType> SimpleMatrixToCopy);// deletes everything, copies data. Use to change the type of one matrix to another

        void SetAll(TemplateType SetValue);

//    TemplateType Value(vector<int> Index);//returns the value of the matrix given the arguments
//    TemplateType Value(int LinearIndex);//used for 1-d matrices and for speed in copying functions
//    TemplateType ValueLinearIndex(int LinearIndex);//used for 1-d matrices and for speed in copying functions
//    TemplateType Value(bool CheckDimension, vector<int> Index);
//    template <typename... ArgType>
//    TemplateType Value(int FirstIndex, int SecondIndex, ArgType... args);//special user specified version


        TemplateType UnitaryValue(int FirstIndex, ...);//this is used to return all values including those that lies outside the dimension of the matrix in the singular dimension

        template <typename... ArgType>
        void Resize(int FirstIndex, ArgType... args);//increases dimensions by specified amount
        SimpleMatrix Transpose(void);//takes 1 or 2 dimension matrices only

        vector<TemplateType> ConvertToVector(void);// if the matrix only has one dimension greater than 1: convert that dimension to a vector



    // Overloading, pointer and template functions
    // Good tutorial on how this works: http://www.cprogramming.com/tutorial/operator_overloading.html
    // Index overloading
    // use () instead http://stackoverflow.com/questions/2533235/operator-c
    //Getting values
        TemplateType operator()(int i)const{ return ValueArray[CreateIndexLinear(i)];}
        TemplateType operator()(int i, int j)const{ return ValueArray[CreateIndex2D(i, j)];}
        TemplateType operator()(vector<int> IndexVector)const{return ValueArray[IndexPosCheck(IndexVector)];}//for getting values
        template<typename... ArgType>
        TemplateType operator()(int i, int j, ArgType ... args)const
        {
            fill(ThisIndex.begin(), ThisIndex.end(), -1);//Reset vector
            ThisIndexSize=0;
            if (ConvertIndexToVector(i, j, args...))//if it doesn't have excessive indicies
            {
                return ValueArray[IndexPosCheck(ThisIndex)];
            }
            // else if it is too big
            cout<<"Error: too many dimensions specified for this matrix"<<endl;
            exit(-1);
        }


    //Setting Values
        TemplateType & operator()(int i){ return ValueArray[CreateIndexLinear(i)];}//return a pointing for setting values
        TemplateType & operator()(int i, int j){ return ValueArray[CreateIndex2D(i, j)];}
        TemplateType & operator()(vector<int> IndexVector){return ValueArray[IndexPosCheck(IndexVector)];}//for getting values
        template<typename... ArgType>
        TemplateType & operator()(int i, int j, ArgType ... args)
        {
            fill(ThisIndex.begin(), ThisIndex.end(), -1);//Reset vector
            ThisIndexSize=0;
            if (ConvertIndexToVector(i, j, args...))//if it doesn't have excessive indicies
            {
                return ValueArray[IndexPosCheck(ThisIndex)];
            }
            // else if it is too big
            cout<<"Error: too many dimensions specified for this matrix"<<endl;
            exit(-1);
        }






    //Plus
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator+(const SimpleMatrix<OtherType>& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator+(const OtherType& Other);
    //Minus
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator-(const SimpleMatrix<OtherType>& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator-(const OtherType& Other);
        //Unary minus
        SimpleMatrix<TemplateType> operator-(void);
    //Times
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator*(const SimpleMatrix<OtherType>& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator*(const OtherType& Other);
    //Divide
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator/(const SimpleMatrix<OtherType>& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator/(const OtherType& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> DivideByMatrix(const OtherType& Other);
    //Modulus
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator%(const SimpleMatrix<OtherType>& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> operator%(const OtherType& Other);
        template <typename OtherType>
        SimpleMatrix<TemplateType> ModulusByMatrix(const OtherType& Other);


    //Other operators
    //http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B

    // http://www.thegeekstuff.com/2013/09/cpp-operator-overloading/
    //template <typename ReturnTemplateType>
    //SimpleMatrix<ReturnTemplateType> Apply(ReturnTemplateType (*FunctionPointer)(TemplateType));
    void Apply(TemplateType (*FunctionPointer)(TemplateType));

    void Display(void);
    void DisplayInfo(void);



};
///*****************************************************************************************************************
///*****************************************************************************************************************
///*****************************************************************************************************************
///*****************************************************************************************************************
// Indexing functions
///*****************************************************************************************************************

template <typename TemplateType>
int SimpleMatrix<TemplateType>::CreateIndexLinear(int i)
{
    if (i<0 || i>=TotalArraySize)
    {
        cout<<"Error: Accessing outside range of linear index"<<endl;
        cout<<"Attempting to access:   "<<i<<endl;
        cout<<"This matrix total size: "<<TotalArraySize<<endl;
        cout<<"Matrix size is ";
        for (int CurrentDim : DimSize)
            cout<<CurrentDim<<", ";
        cout<<endl;
        exit(-1);
    }
    return i;
}

template <typename TemplateType>
int SimpleMatrix<TemplateType>::CreateIndex2D(int i, int j)
{
    if (NDimSize!=2)
    {
        cout<<"Error: Incorrect dimensions specified in matrix"<<endl;
        cout<<"Number of dimensions of matrix: "<<NDimSize<<endl;
        cout<<"Number of dimensions of index : 2"<<endl;
        exit(-1);
    }
    ThisIndex[0]=i;
    ThisIndex[1]=j;
    return IndexPosCheck(ThisIndex);
}




template <typename TemplateType>
bool SimpleMatrix<TemplateType>::InRange(vector<int> Index) //used privately to determine that the index is in the range sepcified
{
    //Check the size of dimension vectors (want to avoid [4][5][6] vs [1][2])
    if (NDimSize<Index.size())
    {
        //cout<<"Got out in number of dimensions NDimSize: "<<NDimSize<<" Index.size() " << Index.size() << endl;
        return false;
    }
    //Check each of the individual dim
    int DimCount=0;
    for (int CurrentDim : DimSize)
    {
        if (Index[DimCount]<0||(Index[DimCount]>CurrentDim-1))//because the indicies got from 0 to p-1 for a dimension that is p in size
        {
            return false;
        }
        DimCount++;
    }
    return true;
}

template <typename TemplateType>
int SimpleMatrix<TemplateType>::IndexPos(vector<int> Index)//used privately to determine the linear array
{
    int ReturnIndexValue=0;
    int IndexCount=0;
    for (int ThisIndex: Index)
    {
        ReturnIndexValue=ReturnIndexValue+ThisIndex*Base[IndexCount];
        IndexCount++;
    }
    return ReturnIndexValue;
}

template <typename TemplateType>
int SimpleMatrix<TemplateType>::IndexPosCheck(vector<int> Index)
{
    //Determine if in range
    if (InRange(Index)==false)
    {
        cout<<"Error: Index is incorrect size"<<endl;
        cout<<"Input index size is: ";
        for (int CurrentIndex: Index)
            cout<<CurrentIndex<<", ";
        cout<<endl;
        cout<<"Matrix size is:      ";
        for (int CurrentDim : DimSize)
            cout<<CurrentDim<<", ";
        cout<<endl;
        exit(-1);
    }
    return IndexPos(Index);
}






// Contructors
    template <typename TemplateType> template <typename... ArgType>
    SimpleMatrix<TemplateType>::SimpleMatrix(int FirstIndex, ArgType... args)//constructor should force all in specified matrices pass through this function while not diverting other defintitions such as vector<int>
    {
        //Determine number of args
        NumConstructorArgs=0;
        ConstructorArgStorage.clear();
        NDimSize=CountConstructorArgs(FirstIndex, args...);
        DimSize=ConstructorArgStorage;
        ThisIndex.resize(NDimSize);//an unchanging vector for accessing relevant indicies
        //Assign memory size of matrix
        CreateValueArray();

    }

    template <typename TemplateType>
    SimpleMatrix<TemplateType>::SimpleMatrix(vector<int> TempDimSize)
    {
        DimSize=TempDimSize;
        NDimSize=DimSize.size();
        ThisIndex.resize(NDimSize);//an unchanging vector for accessing relevant indicies
        //Assign memory size of matrix
        CreateValueArray();
    }

    template <typename TemplateType>
    SimpleMatrix<TemplateType>::SimpleMatrix(void)//used to define a variable extremely quickly
    {
        DimSize.push_back(1);
        NDimSize=1;
        ThisIndex.resize(NDimSize);//an unchanging vector for accessing relevant indicies
        TotalArraySize=1;
        ValueArray.resize(1);
        Base.push_back(1);
    }

    template <typename TemplateType>
    void SimpleMatrix<TemplateType>::CreateValueArray(void)
    {
        TotalArraySize=1;
        for (int val : DimSize )
        {
            TotalArraySize=TotalArraySize*val;
        }
        //extend the memory space of the storage array
        ValueArray.resize(TotalArraySize);
        //Determine the base for indexing the array
        int BaseMultiplier=1;
        for (int val : DimSize )
        {
            Base.push_back(BaseMultiplier);
            BaseMultiplier=BaseMultiplier*val;
        }
    }






//Dimension return functions
    template <typename TemplateType>
    int SimpleMatrix<TemplateType>::TotalElements(void)//allows
    {
        return TotalArraySize;
    }

    template <typename TemplateType>
    vector<int> SimpleMatrix<TemplateType>::Dim(void)
    {
        return DimSize;
    }







//// Setting functions
//template <typename TemplateType>
//void SimpleMatrix<TemplateType>::Set(TemplateType SetValue, vector<int> Index)
//{
//    ValueArray[IndexPosCheck(Index)]=SetValue;
//}
//
//template <typename TemplateType> template <typename... ArgType>
//void SimpleMatrix<TemplateType>::Set( TemplateType SetValue, int FirstIndex, int SecondIndex, ArgType... args)//allows arbitrary indices to be set
//{
//    NumConstructorArgs=0;
//    ConstructorArgStorage.clear();
//    CountConstructorArgs(FirstIndex, SecondIndex, args...);
//    Set(SetValue, ConstructorArgStorage);//ConstructorArgStorage is the index in vector form
//}
//
//template <typename TemplateType>
//void SimpleMatrix<TemplateType>::Set( TemplateType SetValue, int LinearIndex)//allows 1-D access
//{
//    if (LinearIndex>=0 && LinearIndex<TotalArraySize && NDimSize==1)
//        ValueArray[LinearIndex]=SetValue;
//    else
//    {
//        if (NDimSize>1)
//            cout<<"Linear access to multidimensional matrix attempted. If this was intentional, try .SetLinearIndex "<<endl;
//        else
//            cout<<"Linear index " << LinearIndex << " outside bounds of matrix total size " << TotalArraySize<<endl;
//        exit(-1);
//    }
//}
//
template <typename TemplateType>
void SimpleMatrix<TemplateType>::SetLinearIndex( TemplateType SetValue, int LinearIndex)//allows high speed linear access to the data
{
    if (LinearIndex>=0 && LinearIndex<TotalArraySize)
        ValueArray[LinearIndex]=SetValue;
    else
    {
        cout<<"Linear index " << LinearIndex << " outside bounds of matrix total size " << TotalArraySize<<endl;
        exit(-1);
    }
}

template <typename TemplateType>
void SimpleMatrix<TemplateType>::SetAll(TemplateType SetValue)
{
    for (int i=0; i< TotalArraySize; i++)
        ValueArray[i]=SetValue;
}



//// Value functions
//template <typename TemplateType>
//TemplateType SimpleMatrix<TemplateType>:: Value(vector<int> Index)
//{
//    //Check the index is right
//    return ValueArray[IndexPosCheck(Index)];
//}
//
//template <typename TemplateType>
//TemplateType SimpleMatrix<TemplateType>:: Value(int LinearIndex)//used for 1-D entries
//{
//    if (LinearIndex>=0 && LinearIndex<TotalArraySize && NDimSize==1)
//        return ValueArray[LinearIndex];
//    else
//    {
//        if (NDimSize>1)
//            cout<<"Linear access to multidimensional matrix attempted. If this was intentional, try .ValueLinearIndex "<<endl;
//        else
//            cout<<"Linear index " << LinearIndex << " outside bounds of matrix total size " << TotalArraySize<<endl;
//        exit(-1);
//    }
//}
//template <typename TemplateType>
//TemplateType SimpleMatrix<TemplateType>:: ValueLinearIndex(int LinearIndex)//used for superfast speed
//{
//    if (LinearIndex>=0 && LinearIndex<TotalArraySize)
//        return ValueArray[LinearIndex];
//    else
//    {
//        cout<<"Linear index " << LinearIndex << " outside bounds of matrix total size " << TotalArraySize<<endl;
//        exit(-1);
//    }
//}
//
//template <typename TemplateType> template <typename... ArgType>
//TemplateType SimpleMatrix<TemplateType>::Value(int FirstIndex, int SecondIndex, ArgType... args)//only use if there are 2 or more int indices
//{
//    NumConstructorArgs=0;
//    ConstructorArgStorage.clear();
//    CountConstructorArgs(FirstIndex, SecondIndex, args...);
//    return ValueArray[IndexPosCheck(ConstructorArgStorage)];//Check then look up the linear index specificed by ConstructorArgStorage, find the associated value then return.
//}


template <typename TemplateType> template <typename OtherType>
bool SimpleMatrix<TemplateType>::DimensionsCompatible(SimpleMatrix<OtherType> Other)
{
    if (NDimSize!=Other.NDimSize)
    {
        return false;
    }
    for (int DimCount=0; DimCount<NDimSize; DimCount++)
    {
        //if they are both bigger than 1 and are unequal
        //if (ADim[DimCount]>1 && BDim[DimCount]>1 && (ADim[DimCount]!= BDim[DimCount]))//this functionality may come in later releases
        if ((DimSize[DimCount]!= Other.DimSize[DimCount]))
        {
            return false;
        }
    }
    return true;//no problems
}

template <typename TemplateType> template <typename OtherType>
void SimpleMatrix<TemplateType>::StopIfDimensionsIncompatible(SimpleMatrix<OtherType> Other)
{
    if (DimensionsCompatible(Other)==false)
    {
        cout<<"The dimensions of the matrices do not match"<<endl;
        cout<<"Matrix 1: ";
        for (int ThisDim : DimSize)
            cout<<ThisDim <<", ";
        cout<<endl;
        cout<<"Matrix 2: ";
        for (int OtherDim : Other.DimSize)
            cout<<OtherDim <<", ";
        exit(-1);
    }
}


template <typename TemplateType> template <typename... ArgType>
void SimpleMatrix<TemplateType>::Resize(int FirstIndex, ArgType... args)
{
    DimSize.clear();
    ThisIndex.clear();
    ValueArray.clear();
    Base.clear();

    //Determine number of args
    NumConstructorArgs=0;
    ConstructorArgStorage.clear();
    NDimSize=CountConstructorArgs(FirstIndex, args...);
    DimSize=ConstructorArgStorage;
    ThisIndex.resize(NDimSize);//an unchanging vector for accessing relevant indicies
    //Assign memory size of matrix
    CreateValueArray();
}





/// Operator overloading
// http://en.wikibooks.org/wiki/C%2B%2B_Programming/Operators/Operator_Overloading
// In particular, we will be looking to overload the assignment operator to allow straighforward assignment of values
// http://en.wikibooks.org/wiki/C%2B%2B_Programming/Operators/Operator_Overloading#Assignment_operator
// we will also make use of the subscript operator []
// we may be able to use the subscript operator [][][] by iterating over the subscript operator multiple times
// http://stackoverflow.com/questions/6969881/operator-overload

// This source code has the basis for the overwhelming marjority of the operators that we wish to work on. Just search for "operator"
// https://gcc.gnu.org/onlinedocs/gcc-4.6.3/libstdc++/api/a01115_source.html

// Good tutorial on how this works: http://www.cprogramming.com/tutorial/operator_overloading.html

    ///Plus
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator+(const SimpleMatrix<OtherType>& Other)
        {
            StopIfDimensionsIncompatible(Other);
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)//for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]+Other.ValueArray[i];
            }
            return ResultSM;
        }

        //For other types
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator+(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]+Other;
            }
            return ResultSM;
        }

        //Swapping for a discrete set of functions
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(char Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(short Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(int Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(long Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(long long Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(float Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(double Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(long double Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator+(bool Other, SimpleMatrix<TemplateType> SMClass){return (SMClass + Other);}

    ///Minus
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator-(const SimpleMatrix<OtherType>& Other)
        {
            StopIfDimensionsIncompatible(Other);
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)//for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]-Other.ValueArray[i];
            }
            return ResultSM;
        }

        //For other types
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator-(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]-Other;
            }
            return ResultSM;
        }

        //Unary minus
        template <typename TemplateType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator-(void)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=-ValueArray[i];
            }
            return ResultSM;
        }

        //Swapping for a discrete set of functions
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(char Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(short Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(int Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(long Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(long long Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(float Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(double Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(long double Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator-(bool Other, SimpleMatrix<TemplateType> SMClass){return -(SMClass - Other);}

    ///Times
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator*(const SimpleMatrix<OtherType>& Other)
        {
            StopIfDimensionsIncompatible(Other);
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)//for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]*Other.ValueArray[i];
            }
            return ResultSM;
        }

        //For other types
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator*(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]*Other;
            }
            return ResultSM;
        }

        //Swapping for a discrete set of functions
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(char Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(short Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(int Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(long Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(long long Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(float Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(double Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(long double Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator*(bool Other, SimpleMatrix<TemplateType> SMClass){return (SMClass * Other);}

    ///Divide
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator/(const SimpleMatrix<OtherType>& Other)
        {
            StopIfDimensionsIncompatible(Other);
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)//for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]/Other.ValueArray[i];
            }
            return ResultSM;
        }

        //For other types
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator/(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]/Other;
            }
            return ResultSM;
        }

        //Swapping for a discrete set of functions
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::DivideByMatrix(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=Other/ValueArray[i];
            }
            return ResultSM;
        }
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(char Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(short Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(int Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(long Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(long long Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(float Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(double Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(long double Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator/(bool Other, SimpleMatrix<TemplateType> SMClass){return SMClass.DivideByMatrix(Other);}

    ///Modulus
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator%(const SimpleMatrix<OtherType>& Other)
        {
            StopIfDimensionsIncompatible(Other);
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)//for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]%Other.ValueArray[i];
            }
            return ResultSM;
        }

        //For other types
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::operator%(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=ValueArray[i]%Other;
            }
            return ResultSM;
        }

        //Swapping for a discrete set of functions
        template <typename TemplateType> template <typename OtherType>
        SimpleMatrix<TemplateType> SimpleMatrix<TemplateType>::ModulusByMatrix(const OtherType& Other)
        {
            SimpleMatrix<TemplateType> ResultSM(DimSize);
            for (int i=0; i<TotalArraySize; i++)    //for all the elements
            {
                ResultSM.ValueArray[i]=Other%ValueArray[i];
            }
            return ResultSM;
        }
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(char Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(short Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(int Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(long Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(long long Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(float Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(double Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(long double Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}
        template <typename TemplateType> SimpleMatrix<TemplateType> operator%(bool Other, SimpleMatrix<TemplateType> SMClass){return SMClass.ModulusByMatrix(Other);}



//Apply a function to the matrix
template <typename TemplateType>
void SimpleMatrix<TemplateType>::Apply(TemplateType (*FunctionPointer)(TemplateType))
{
    TemplateType TempResultStore;
    for (int i=0; i<TotalArraySize; i++)//for all the elements
    {
        ValueArray[i]=FunctionPointer(ValueArray[i]);
    }
}

//This section works
//template <typename InputTemplateType>
//SimpleMatrix<InputTemplateType> Apply(InputTemplateType (*FunctionPointer)(InputTemplateType), SimpleMatrix<InputTemplateType> A)
//This section used to work, now broken
template <typename ReturnTemplateType, typename InputTemplateType>
SimpleMatrix<ReturnTemplateType> Apply(ReturnTemplateType (*FunctionPointer)(InputTemplateType), SimpleMatrix<InputTemplateType> A)
{
    ReturnTemplateType TempResultStore;
    //Determine size of input vector
    SimpleMatrix<ReturnTemplateType> ResultSM(A.Dim());

    //for all the elements of A
    int SizeOfA=A.TotalElements();
    for (int i=0; i<SizeOfA; i++)
    {
        TempResultStore=FunctionPointer(A(i));
        cout<<"Output: "<<TempResultStore<<" "<<A(i)<<endl;
        ResultSM.SetLinearIndex(TempResultStore, i);
    }
    return ResultSM;
}

//Applying a function to 2 multidimensional matrices
template <typename ReturnTemplateType, typename InputTemplateType>
SimpleMatrix<ReturnTemplateType> Apply(ReturnTemplateType (*FunctionPointer)(InputTemplateType, InputTemplateType), SimpleMatrix<InputTemplateType> A, SimpleMatrix<InputTemplateType> B)
{
    A.StopIfDimensionsIncompatible(B);
    ReturnTemplateType TempResultStore;
    SimpleMatrix<ReturnTemplateType> ResultSM(A.Dim());

    //for all the elements of A
    int SizeOfA=A.TotalElements();
    for (int i=0; i<SizeOfA; i++)
    {
        TempResultStore=FunctionPointer(A.ValueLinearIndex(i), B.ValueLinearIndex(i));
        ResultSM.SetLinearIndex(TempResultStore, i);
    }
    return ResultSM;
}



//Display functions
template <typename TemplateType>
void SimpleMatrix<TemplateType>::Display(void)
{
    cout<<"Contents: ";
    for (int i=0; i< TotalArraySize; i++)
        cout<<ValueArray[i]<<", ";
    cout<<endl;
}

template <typename TemplateType>
void SimpleMatrix<TemplateType>::DisplayInfo(void)
{
    cout<<endl<<"Total element count: "<< TotalArraySize<<endl;
    cout<<"Dimensions: ";
    for (int val : DimSize )
        cout<< val <<", ";
    cout<<endl;
        cout<<"Base values for indexing the content in multi-dimensions"<<endl;
    for (int val : Base )
        cout<< val <<", ";
    cout<<endl;
    cout<<"Contents: ";
    for (int i=0; (i< TotalArraySize) && i<5 ; i++)//only a short sample
        cout<<ValueArray[i]<<", ";
    cout<<"..."<<endl;
    cout<<"To see full contents use Display()"<<endl<<endl;
}














template <typename TemplateType>
SimpleMatrix<TemplateType> Join(SimpleMatrix<TemplateType> A, SimpleMatrix<TemplateType> B, int Dimension)
{
    SimpleMatrix<TemplateType> ReturnMatrix;
    //check that all other dimensions are equal (except the dimension that we want to join along)
    //for (int ThisDim :A.Dimensions)
    return ReturnMatrix;
}


    //Use this in in CountConstructorArgs to only run if ints
    //http://www.cplusplus.com/reference/type_traits/enable_if/
