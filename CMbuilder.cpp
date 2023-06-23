#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(void)
{
    srand(time(NULL));
    ifstream degrees;
    degrees.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\degreesreserva.txt");
    string name; //im not saving the names of the nodes, this is a variable that will be overwritten and not used
    int deg[4000]; //>N

    int i, N, E;
    i=0;
    E=0;
    while(!degrees.eof())
    {
        degrees >> name >> deg[i];
        E=E+deg[i];
        i++;
    }
    N=i;
    degrees.close();

    int stub[E]; //stub list
    int reserva[E];
    int j, degt;
    i=0;
    degt=deg[i];
    j=0;
    while(j<E)
    {
        if(j>=degt)
        {
            if(i>=N){goto SAL;}
            i++;
            degt=degt+deg[i];
        }

        stub[j]=i;
        reserva[j]=stub[j];
        j++;
    }
    SAL:

    int k;
    int repeticiones, Ereserva;
    Ereserva=E;
    repeticiones=1;
    int a, nodea, b, nodeb;
    ofstream edgelist;
    for(k=0;k<repeticiones;k++)
    {
        E=Ereserva;
        for(j=0;j<E;j++)
        {
            stub[j]=reserva[j];
        }
        edgelist.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\CMedgelist"+to_string(k)+".txt");
        while(E>0)
        {
            //choose a node
            a=rand()*E/RAND_MAX;
            //remove it from the stub list
            nodea=stub[a];
            stub[a]=stub[E-1];
            E=E-1;
            //choose another
            b=rand()*E/RAND_MAX;
            nodeb=stub[b];
            stub[b]=stub[E-1];
            E=E-1;
            //link them
            edgelist << nodea << "  " << nodeb << endl;
        }
        edgelist.close();
    }

    return 0;
}
