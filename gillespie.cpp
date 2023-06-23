#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <ctime>


using namespace std;

int main(void)
{
    //read the edgelist and generate the useful vectors
    //-----------------------------------------
    string filename;
    //cout << "Please, indicate the name of the edgelist (including extension), located in this directory: ";
    //cin >> filename;
    filename="edgelistint.txt"; //"net1m_2009-04-01_GC.edge"   either change the filename here or introduce via the console with the previous two lines and commenting this one
    //quitar si hay lineas comentadas al principio de la edgelist

    //first i read the nodes and count their degrees
    int N; //number of nodes    since im reading the edgelist i dont see isolated nodes, this N is <= real N
    int E; //number of edges
    int lectura[80000]; //the size of the edgelist cant be bigger than this for this program with strings, remake this section of the program for the S1 case
    int node[11000]; //>N,    stores the names of the nodes
    int deg[11000]; //stores the degrees of the nodes
    ofstream properties;
    properties.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\part2\\properties.txt");
    properties << "This file contains information of the network given by: " << filename << endl << endl;
    cout << "The file 'properties.txt' contains several properties of the network." << endl;

    int i,j, k, l; //i y j pertenecen a lectura, k y l a node
    bool isunique;
    int contador[80000]; //
    ifstream edgelist;
    edgelist.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\part2\\"+filename);

    //leo todos los nodos
    i=0;
    edgelist >> lectura[i];
    while(!edgelist.eof())
    {
        i++;
        edgelist >> lectura[i];
    }
    edgelist.close();
    E=(i+1)/2;  //porque i empieza en 0

    //paso a mi vector de nombres de nodos y cuento degrees
    k=0;
    node[k]=lectura[0];
    deg[k]=1;
    contador[0]=0;

    for(i=1;i<2*E;i++)
    {
        isunique=true;

        for(j=0;j<i;j++)
        {
            if(lectura[j]==lectura[i])
            {
                //tengo que comprobar no repetir edges al contar el degree
                if(i%2==0)  //i even, estoy leyendo el primer elemento de un edge, tengo que comprobar el siguiente con el que va con j
                {
                    if(j%2==0) //j tambien el primero, comparo con el siguiente
                    {
                        if(lectura[i+1]!=lectura[j+1]){deg[j-contador[j]]++;}
                    }else if(j>0){ //j odd, comparo con el anterior
                        if(lectura[i+1]!=lectura[j-1]){deg[j-contador[j]]++;}
                    }
                }else{ //i odd, comparo el anterior a i con los j
                    if(j%2==0) //j el primero, comparo con el siguiente
                    {
                        if(lectura[i-1]!=lectura[j+1]){deg[j-contador[j]]++;}
                    }else if(j>0){ //j odd, comparo con el anterior
                        if(lectura[i-1]!=lectura[j-1]){deg[j-contador[j]]++;}
                    }
                }
                isunique=false;
                goto fuera;
            }
        }
        fuera:

        if(isunique)
        {
            k++;
            node[k]=lectura[i];
            deg[k]=1;
        }
        contador[i]=i-k;
    }
    N=k+1;

    properties << "Number of nodes: " << N << endl << "Number of edges: " << E << endl;

    double kav; //average degree
    kav=(double)2*E/(double)N;

    properties << "Average degree: " << kav << endl;

    //primero tengo que traducir lectura (edgelist(string) con nombres de los nodos) a edge (edgelist(int) con números de los nodos)
    int edge[2*E];
    for(i=0;i<2*E;i++)
    {
        j=0;
        while(lectura[i]!=node[j])
        {
            j++;
        }
        edge[i]=j;
    }

    int v[2*E]; //stores the edgelist information
    int pointerini[N], pointerfin[N];
    pointerini[0]=0;
    bool repe;
    for(i=0;i<N;i++)  //for each node
    {
        pointerini[i+1]=pointerini[i]+deg[i];
        pointerfin[i]=pointerini[i];

        for(k=0;k<2*E;k++) //busco en edgelist
            {
                if(lectura[k]==node[i])
                {
                    if(k%2==0)  //k even, estoy leyendo el primer elemento de un edge, tengo que cojer el siguiente
                    {
                        //compruebo no repetir con los anteriores (si hay)
                        repe=false;
                        for(j=pointerini[i];j<pointerfin[i];j++)
                        {
                            if(edge[k+1]==v[j]){repe=true;}
                        }
                        if(!repe)
                        {
                            v[pointerfin[i]]=edge[k+1];
                          //  edgelistint << i << "    " << v[pointerfin[i]] << endl;
                            pointerfin[i]++;
                        }

                    }else { //k odd, cojo el anterior
                        //compruebo no repetir con los anteriores (si hay)
                        repe=false;
                        for(j=pointerini[i];j<pointerfin[i];j++)
                        {
                            if(edge[k-1]==v[j]){repe=true;}
                        }
                        if(!repe)
                        {
                            v[pointerfin[i]]=edge[k-1];
                       //     edgelistint << v[pointerfin[i]] << "    " << i << endl;
                            pointerfin[i]++;
                        }
                    }
                }
                if((pointerfin[i]-pointerini[i])==deg[i]){goto salgo;}
            }
            salgo:
            k=0; //esto no vale pa na pero si no pongo algo después del : no compila
    }

//------------------------------------------

    //gillespie---------------------
    //vectores necesarios
    int ninfected[N]; //nodos infectados
    int pn; //pointer of the latter vector, number of infected nodes = pn + 1
    pn=-1; //a la hora de infectar, primero actualizar el pointer. al recuperar, después.
    int eact[E][2]; //lista de links activos
    int pe; //pointer of the latter vector, number of active links = pe + 1
    pe=-1; //a la hora de infectar, primero actualizar el pointer. al recuperar, después.
    int guide[2*E]; //guide that saves the location of an edge in eact[] based on v[]
    srand(time(NULL));

    //condicion inicial, infecto un x% de nodos
    double initialpercent;
    initialpercent=0.20;

    bool infectado;
    for(i=0;i<N;i++)    //infecto nodos
    {
        if((double)rand()/(double)RAND_MAX<initialpercent)
        {
            pn++;
            ninfected[pn]=i;
        }
    }
    int a;
    //links activos iniciales
    for(i=0;i<(pn+1);i++) //recorro los infectados
    {
        for(j=pointerini[ninfected[i]];j<pointerfin[ninfected[i]];j++) //para sus vecinos, no se va a repetir
        {
            infectado=false;
            for(k=0;k<(pn+1);k++) //si el vecino está infectado
            {
                if(v[j]==ninfected[k])
                {
                    infectado=true;
                }
            }
            if(!infectado)  //si no se ha infectado, ese link es activo
            {
                pe++;
                eact[pe][0]=ninfected[i];
                eact[pe][1]=v[j];
                guide[j]=pe;
                //guide is symmetrical
                for(a=pointerini[v[j]];a<pointerfin[v[j]];a++)
                {
                    if(v[a]==ninfected[i])
                    {
                        guide[a]=pe;
                    }

                }
            }
        }
    }

    //ciclo de gillespie
    double tiempo, t, rando;
    tiempo=5000; //tiempo total de simulacion (tiempo real)
    t=0;
    double lambda;
    lambda=0.3;
    int recuperado;
    //density
    ofstream density;
    density.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\part2\\density.txt");
    cout << "The file density.txt contains the variation of the density of infected nodes for the time duration." << endl;
    density << "#time   fraction of infected" << endl << t << "   " << (double)(pn+1)/(double)N << endl;
    while(t<tiempo)
    {
        if((pn<0)||(pn>N-2)||(pe<0)){goto exit;}else
        {
            if((double)rand()/(double)RAND_MAX<(double)(pn+1)/(double)(pn+1+lambda*(pe+1))) //si hay recuperación
            {
                //escojo un nodo al azar y lo recupero
                i=(double)rand()/(double)RAND_MAX*pn;
                recuperado=ninfected[i];
                ninfected[i]=ninfected[pn];
                ninfected[pn]=-1;  //dejo un -1 donde ya no hay nodo infectado
                pn=pn-1;
                //actualizo links activos
                //busco los vecinos del nodo recuperado en v
                for(j=pointerini[recuperado];j<pointerfin[recuperado];j++)
                {
                    //si infectado, añado link activo
                    infectado=false;
                    for(k=0;k<(pn+1);k++) //si el vecino está infectado
                    {
                        if(v[j]==ninfected[k])
                        {
                            infectado=true;
                        }
                    }
                    if(infectado)  //si es infectado, ese link es activo
                    {
                        pe++;
                        eact[pe][0]=v[j];
                        eact[pe][1]=recuperado;
                        guide[j]=pe;
                        //guide is symmetrical
                        for(a=pointerini[v[j]];a<pointerfin[v[j]];a++)
                        {
                            if(v[a]==recuperado)
                            {
                                guide[a]=pe;
                            }

                        }
                    }else   //si no, era activo y ya no, lo quito
                    {
                        //cout << v[j] << "   " << eact[guide[j]][1] << endl;  //HERE I SEE A MISTAKE after a few iterations
                        eact[guide[j]][0]=eact[pe][0];
                        eact[guide[j]][1]=eact[pe][1];
                        eact[pe][0]=-1;
                        eact[pe][1]=-1;
                        pe=pe-1;
                    }
                }

            }else   //si hay infeccion
            {
                //escojo un link activo al azar e infecto al nodo sano de los dos, es el de la coordenada 1 porque los he puesto asi
                i = eact[(int)((double)rand() / RAND_MAX * pe)][1];
                pn++;
                ninfected[pn]=i;
                //actualizo links activos
                //busco los vecinos del nodo (ahora infectado) en v
                for(j=pointerini[i];j<pointerfin[i];j++)
                    {
                        //si no infectado, añado link activo
                        infectado=false;
                        for(k=0;k<(pn+1);k++) //si el vecino está infectado
                        {
                            if(v[j]==ninfected[k])
                            {
                                infectado=true;
                            }
                        }
                        if(!infectado)  //si no es infectado, ese link es activo
                        {
                            pe++;
                            eact[pe][0]=i;
                            eact[pe][1]=v[j];
                            guide[j]=pe;
                            //guide is symmetrical
                            for(a=pointerini[v[j]];a<pointerfin[v[j]];a++)
                            {
                                if(v[a]==i)
                                {
                                    guide[a]=pe;
                                }

                            }
                        }else   //si es infectado, lo quito
                        {
                            eact[guide[j]][0]=eact[pe][0];
                            eact[guide[j]][1]=eact[pe][1];
                            eact[pe][0]=-1;
                            eact[pe][1]=-1;
                            pe=pe-1;
                        }
                    }
              }
        }

        exit:
        //actualizo el tiempo
        do
        {
            rando=(double)rand()/(double)RAND_MAX;
        }while(rando==0);
        t=t-(double)log(rando)/(double)(1+lambda);

        //record density (normalized)
        density << t << "   " << (double)(pn+1)/(double)N << endl;
    }

    density.close();
    properties.close();

    return 0;
}
