#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

int main(void)
{
    //read the edgelist and generate the useful vectors
//-----------------------------------------
    string filename;
    //cout << "Please, indicate the name of the edgelist (including extension), located in this directory: ";
    //cin >> filename;
    filename="edgelist.edgelist"; //"edgelist.edgelist" "CMedgelist0.txt" "edgelistS1.txt" either change the filename here or introduce via the console with the previous two lines and commenting this one
    //quitar si hay lineas comentadas al principio de la edgelist

    //first i read the nodes and count their degrees
    int N; //number of nodes
    int E; //number of edges
    string lectura[40000]; //the size of the edgelist cant be bigger than this for this program with strings, remake this section of the program for the S1 case
    string node[1100]; //>N,    stores the names of the nodes
    int deg[1100]; //stores the degrees of the nodes
    ofstream properties;
    properties.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\properties.txt");
    properties << "This file contains information of the network given by: " << filename << endl << endl;
    cout << "The file 'properties.txt' contains several properties of the network." << endl;

    int i,j, k, l; //i y j pertenecen a lectura, k y l a node
    bool isunique;
    int contador[40000];
    ifstream edgelist;
    edgelist.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\"+filename);

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
    ofstream edgelistint;  //genera un archivo edgelist(int) que es más manejable por otros programas
    edgelistint.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\edgelistint.txt");
    for(k=0;k<2*E;k=k+2)
    {
        edgelistint << edge[k]  << "  " << edge[k+1] << endl;
    }
    edgelistint.close();
    ofstream degrees; //genera un archivo con una lista de los nodos y sus degrees
    degrees.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\degrees.txt");
    //count the number of nodes in each degree class------------------
    int degmax;
    degmax=0;
    for(i=0;i<N;i++)
    {
        degrees << node[i]  << "  " << deg[i] << endl;
        if(deg[i]>degmax){degmax=deg[i];}
    }
    degrees.close();
    cout << "The file 'degrees.txt' contains the nodes of the network and their respective degrees." << endl << "The file 'edgelistint.txt' contains the edgelist of the network with the nodes as integers for manageability." << endl;

    int nodesdeg[degmax]; //stores number of nodes in each degree class
    for(i=0;i<degmax;i++)
    {
        nodesdeg[i]=0;
        for(j=0;j<N;j++)
        {
            if(deg[j]==i){nodesdeg[i]++;}
        }
    }
    //---------------------

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
    //(average) nearest neighbors degree
    double neardeg[degmax];
    double neardegav[degmax];
    ofstream neardegavdist;
    neardegavdist.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\nearavdeg.txt");
    cout << "The file 'nearavdeg.txt' contains the average nearest neighbour degrees for each degree." << endl;
    neardegavdist << "#k    average nearest neighbour degree" << endl;
    for(k=0;k<degmax;k++) //for each deg class
    {
        neardegav[k]=0;
        for(i=0;i<N;i++) //for each node belonging to a degree class
        {
            if(deg[i]==k)
            {
                neardeg[k]=0;
                for(j=pointerini[i];j<pointerfin[i];j++) //for its neighbours
                {
                    neardeg[k]=neardeg[k]+deg[j];
                }
                neardeg[k]=(double)neardeg[k]/(double)deg[i];
                neardegav[k]=neardegav[k]+neardeg[k];
            }
        }
        neardegav[k]=(double)neardegav[k]/(double)nodesdeg[k];
        neardegavdist << k << " " << neardegav[k] << endl;
    }
    neardegavdist.close();

    //------------------------------------------
    //clustering
    int triang, a;
    double c[N]; //clustering
    double ct; //average clustering
    ct=0;
    ofstream clustering;
    clustering.open("C:\\Users\\Usuario\\Desktop\\MasterUB\\Docencia\\ComplexNetworks\\network\\clustering.txt");
    cout << "The file 'clustering.txt' contains the clustering coefficient as a function of the degree of the nodes." << endl;
    clustering << "#deg[i]    clustering" << endl;

    for(i=0;i<N;i++)
    {
        triang=0;
        //para cada nodo i, compruebo las connexiones entre pares de vecinos
        //solo si su degree es mayor que 1, si no ya se que tiene 0 triang
        if(deg[i]>1)
        {
            for(j=pointerini[i]+1;j<(pointerini[i]+deg[i]);j++)
            {
                a=v[j];
                for(k=pointerini[i];k<j;k++)
                {
                    for(l=pointerini[a];l<(pointerini[a]+deg[a]);l++)
                    {
                        if(v[k]==v[l])
                        {
                            triang=triang+1;
                        }
                    }
                }
            }

        c[i]=2*(double)triang/(double)(deg[i]*(deg[i]-1));
        }else{c[i]=0;}
        clustering << deg[i] << "    " << c[i] << endl;
        ct=ct+c[i];

    }
    ct=ct/N;

    clustering.close();
    properties << "Average clustering: " << ct << endl;

    properties.close();

    return 0;
}
