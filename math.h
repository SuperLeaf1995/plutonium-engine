#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

long double * cosTable;
long double * sinTable;
long double * tanTable;

int tableSize;

void Pl_MathInit(int precision);
void Pl_MathEnd(void);

#define Pl_MathCos(x) cosTable[(int)(x*(tableSize/180.0f))]
#define Pl_MathSin(x) sinTable[(int)(x*(tableSize/180.0f))]
#define Pl_MathTan(x) tanTable[(int)(x*(tableSize/180.0f))]

#endif // MATH_H_INCLUDED
