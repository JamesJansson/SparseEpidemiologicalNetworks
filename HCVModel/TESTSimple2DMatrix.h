
#include <math.h>
#include <time.h>
#include <stdio.h>

    SimpleMatrix<float> D;//declaring outside allows you to use up to the total 2GB available to any 32bit program
    SimpleMatrix<float> E;
    SimpleMatrix<float> F;


double Add1(double A)
{
    A=A+1;
    return A;
}
double MultAdd1(double A, double B){return (A*B+1);}

void TESTSimple2DMatrix(void)
{
    clock_t startTime;
    double millisecondsPassed;


    /// Constructors
//    SimpleMatrix(int xsize, int ysize);//alows a vector to be used to specify the dimensions of the matrix
    SimpleMatrix<double> A(2, 3);
    cout<<"2x3"<<endl;
    A.DisplayInfo();
//    SimpleMatrix(void);
    SimpleMatrix<double> U;
    cout<<"1x1"<<endl;
    U.DisplayInfo();
//    SimpleMatrix(vector<int> dimensions);//alows a vector to be used to specify the dimensions of the matrix
    vector<int> SizingVector;
    SizingVector.push_back(5);
    SizingVector.push_back(6);
    SimpleMatrix<double> B(SizingVector);
    cout<<"5x6 vector"<<endl;
    B.DisplayInfo();
//    void Resize(int xsize, int ysize);
    B.Resize(3, 4);
    cout<<"3x4 Resize down using ints"<<endl;
    B.DisplayInfo();
//    void Resize(vector<int> dimensions);
    SizingVector[0]=3;
    SizingVector[1]=4;
    A.Resize(SizingVector);
    cout<<"3x4 Resize up using vector"<<endl;
    A.DisplayInfo();
//
    B(0, 3)=45;
    B.DisplayInfo();

    /// Dimensions
//    vector<int> dim(void);
    vector<int> DimReturn;
    DimReturn=B.dim();
    cout<<"Vector dim output "<<DimReturn[0]<<", "<<DimReturn[1]<<endl;
//    int xsize(void);
//    int ysize(void);
    cout<<"Int dim output "<<B.xsize()<<", "<<B.ysize()<<endl;
//    template <typename OtherType>
//    bool DimensionsCompatible( SimpleMatrix<OtherType> Other);
    if (B.DimensionsCompatible(A))
        cout<<"Dimensions compatible, as expected. Now resize"<<endl;

    A.Resize(5, 4);
    if (!B.DimensionsCompatible(A))
        cout<<"Too big in x works"<<endl;
    A.Resize(2, 4);
    if (!B.DimensionsCompatible(A))
        cout<<"Too small in x works"<<endl;
    A.Resize(3, 5);
    if (!B.DimensionsCompatible(A))
        cout<<"Too big in y works"<<endl;
    A.Resize(3, 2);
    if (!B.DimensionsCompatible(A))
        cout<<"Too small in y works"<<endl;

//    template <typename OtherType>
//    void StopIfDimensionsIncompatible(SimpleMatrix<OtherType> Other);
    //B.StopIfDimensionsIncompatible(A);
    //Works

//    // void StopIfDimensionsIncompatible( SimpleMatrix<OtherType> Other);
//    void SetAll(TemplateType SetValue);
    A.SetAll(55);
    A.DisplayInfo();

//    SimpleMatrix<TemplateType> Transpose(void);
    A.Resize(3, 4);
    for (int i=0; i<(3); i++)
        for (int j=0; j<(4); j++)
            A(i, j)=10*i+j;
    A.DisplayInfo();
    A.Transpose();
    cout<<"Transpose of the above"<<endl;
    A.DisplayInfo();



    ///Getting values
//        TemplateType operator()(int i, int j)const{ return values[i][j];};
//
    cout<<"The number should say 23: "<<A(2, 3)<<endl;
    ///Setting Values
//        TemplateType & operator()(int i, int j){ return values[i][j];};
//  Tested above
//
    //Plus
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator+(const SimpleMatrix<OtherType>& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator+(const OtherType& Other);
    cout<<"A: "<<endl;
    A.DisplayInfo();

    cout<<"Testing int add"<<endl;
    int Two=2;
    B=A+Two;
    B.DisplayInfo();
    cout<<"Testing left int add"<<endl;
    B=Two+A;
    B.DisplayInfo();

    cout<<"Testing float add"<<endl;
    float TreefiddyF=3.5;
    B=A+TreefiddyF;
    B.DisplayInfo();

    cout<<"Testing double add"<<endl;
    double TreefiddyD=3.5;
    B=A+TreefiddyD;
    B.DisplayInfo();

    SimpleMatrix<double> C;
    cout<<"Testing matrix add"<<endl;
    C=A+B;
    C.DisplayInfo();

    //Minus
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator-(const SimpleMatrix<OtherType>& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator-(const OtherType& Other);
//        //Unary minus
//        SimpleMatrix<TemplateType> operator-(void);
    cout<<"A: "<<endl;
    A.DisplayInfo();

    cout<<"Testing int minus"<<endl;
    B=A-Two;
    B.DisplayInfo();
    cout<<"Testing left int minus"<<endl;
    B=Two-A;
    B.DisplayInfo();

    cout<<"Testing float minus"<<endl;
    B=A-TreefiddyF;
    B.DisplayInfo();

    cout<<"Testing double minus"<<endl;
    B=A-TreefiddyD;
    B.DisplayInfo();

    cout<<"Testing matrix minus"<<endl;
    C=A-B;
    C.DisplayInfo();

    cout<<"Testing unitary minus"<<endl;
    B=-A;
    B.DisplayInfo();

    //Times
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator*(const SimpleMatrix<OtherType>& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator*(const OtherType& Other);

    cout<<"A: "<<endl;
    A.DisplayInfo();

    cout<<"Testing int times"<<endl;
    B=A*Two;
    B.DisplayInfo();
    cout<<"Testing left int times"<<endl;
    B=Two*A;
    B.DisplayInfo();

    cout<<"Testing float times"<<endl;
    B=A*TreefiddyF;
    B.DisplayInfo();

    cout<<"Testing double times"<<endl;
    B=A*TreefiddyD;
    B.DisplayInfo();

    cout<<"Testing matrix times"<<endl;
    C=A*B;
    C.DisplayInfo();
    //Divide
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator/(const SimpleMatrix<OtherType>& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator/(const OtherType& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> DivideByMatrix(const OtherType& Other);

    cout<<"A: "<<endl;
    A.DisplayInfo();

    cout<<"Testing int divide"<<endl;
    B=A/Two;
    B.DisplayInfo();
    cout<<"Testing left int divide"<<endl;
    B=Two/A;
    B.DisplayInfo();

    cout<<"Testing float divide"<<endl;
    B=A/TreefiddyF;
    B.DisplayInfo();

    cout<<"Testing double divide"<<endl;
    B=A/TreefiddyD;
    B.DisplayInfo();

    cout<<"Testing matrix divide"<<endl;
    C=A/B;
    C.DisplayInfo();


    //Modulus
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator%(const SimpleMatrix<OtherType>& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> operator%(const OtherType& Other);
//        template <typename OtherType>
//        SimpleMatrix<TemplateType> ModulusByMatrix(const OtherType& Other);
//

//    //Changes this matrix
//    void Apply(TemplateType (*FunctionPointer)(TemplateType));
//    //returns a new matrix
//    template <typename ReturnTemplateType>
//    friend SimpleMatrix<ReturnTemplateType> Apply(ReturnTemplateType (*FunctionPointer)(TemplateType), const SimpleMatrix<TemplateType> A);
//    //returns a new matrix, two input matrices
//    template <typename ReturnTemplateType>
//    friend SimpleMatrix<ReturnTemplateType> Apply(ReturnTemplateType (*FunctionPointer)(TemplateType, TemplateType), const SimpleMatrix<TemplateType> A, const SimpleMatrix<TemplateType> B);
    cout<<"Applying add 1 to A"<<endl;
    A.Apply(Add1);
    A.DisplayInfo();

    B=Apply(Add1, A);
    B.DisplayInfo();

    C=Apply(MultAdd1, A, B);
    C.DisplayInfo();

    cout<<"Testing the joining of matrices"<<endl;

    A.Resize(5, 6);
    B.Resize(4, 6);
    C=XJoin(A, B);
    C.DisplayInfo();

    //Testing that it looks right
    int Axsize, Aysize, Bxsize, Bysize;
    Axsize=3;
    Aysize=4;
    Bxsize=4;
    Bysize=4;
    A.Resize(Axsize, Aysize);
    for (int i=0; i<Axsize; i++)
        for (int j=0; j<Aysize; j++)
            A(i, j)=3000+10*i+j;
    B.Resize(Bxsize, Bysize);
    for (int i=0; i<Bxsize; i++)
        for (int j=0; j<Bysize; j++)
            B(i, j)=6000+10*i+j;

    C=XJoin(A, B);
    C.DisplayInfo();

    C=YJoin(A, B);
    C.DisplayInfo();

    bool DoSpeedTest;
    if (DoSpeedTest)
    {
        cout<<"Starting speed run, check memory space"<<endl;

        int xdim=10000;
        int ydim=10000;
        D.Resize(xdim, ydim);
        E.Resize(xdim, ydim);

        int DSize=sizeof(D);
        cout<<"The size of a 10000x10000 vector is "<<DSize<<endl;

        for (int i=0; i<(xdim); i++)
            for (int j=0; j<(ydim); j++)
                D(i, j)=ydim*i+j;


        cout<<"Adding"<<endl;
        startTime=clock();
        E=D+2;
        millisecondsPassed = (clock() - startTime) / (CLOCKS_PER_SEC/1000);
        cout<<"Milliseconds: "<<millisecondsPassed<<endl;

        cout<<"Dividing"<<endl;
        startTime=clock();
        F=D/E;
        millisecondsPassed = (clock() - startTime) / (CLOCKS_PER_SEC/1000);
        cout<<"Milliseconds: "<<millisecondsPassed<<endl;

        cout<<"Dividing into preallocated space"<<endl;// is prealloacted becaur C is currently the right size
        F.SetAll(0.5);
        startTime=clock();
        F=D/E;
        millisecondsPassed = (clock() - startTime) / (CLOCKS_PER_SEC/1000);
        cout<<"Milliseconds: "<<millisecondsPassed<<endl;
    }







    //char buffer;
    //cin>>buffer;
}




