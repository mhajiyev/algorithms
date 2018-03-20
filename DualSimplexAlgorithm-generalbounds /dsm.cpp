#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Eigen/Dense"
#include "Eigen/LU"
#include <iomanip>
#include <stdlib.h>
#include <map>

using namespace Eigen;
using namespace std;


int main(int argc,char ** argv) {
        std::string input = argv[1];
        std::string output = argv[2];
        std::ifstream inputFile{input};
        double reader;
        int m,n;


        inputFile>>m>>n;
        int withoutArtificial = n;
        n = n + m;                   // Adding artificial variables
        MatrixXd constraints(m,n);
        constraints.setZero(m,n);
        VectorXd c(n),upper(n),lower(n),b(m);
        c.setZero(n);
        upper.setZero(n);
        lower.setZero(n);
        b.setZero(m);


    //Reading from stdin

    for (int i=0;i<withoutArtificial;i++){
        inputFile>>reader;
        c(i) = reader;
    }
    for (int i=0;i<withoutArtificial;i++){
        inputFile>>reader;
        lower(i) = reader;
    }
    for (int i=0;i<withoutArtificial;i++){
        inputFile>>reader;
        upper(i) =reader;
    }



    for (int j = 0; j < m;j++){
        for (int i = 0;i < withoutArtificial;i++) {
            inputFile >> reader;
            constraints(j,i) = reader;
        }
    }

    for (int i=0;i<m;i++){
        inputFile>>reader;
        b(i) = reader;
    }

    for (int k = 0; k<m;k++){
        constraints(k,withoutArtificial+k) = 1;     // Creating an initial basis from slack variables
    }

    std::vector <int> status;
    for (int i = 0;i < n;i++){
        status.push_back(0);            //
    }

    for (int i = 0; i < withoutArtificial; i++){
        if (c(i)>=0) status.at(i) = -1;       // Each column is either -1,0,1. -1 for lower bound,1 for upper bound, 0 for basic vars
        else status.at(i) = 1;
    }


    double epsilon = 0.000001;
    ofstream myfile;
    myfile.open(output);

//Reading from input is finished

//----------------------------------------------------

    VectorXd C_basis(m);
    VectorXd basicSolution(m);
    MatrixXd basis (m,m);
    C_basis.setZero(m);
    basicSolution.setZero(m);
    basis.setZero(m,m);
    while (1){

        int lnum =0;
        int unum = 0;
        int bnum =0;
    for (int i = 0; i < n; i++) {
       if (!status.at(i)) {C_basis(bnum) = c(i);bnum++;}
        else if (status.at(i)==1) unum++;
        else lnum++;
    }

    MatrixXd L (m,lnum),U (m,unum);
        L.setZero(m,lnum);
        U.setZero(m,unum);
    VectorXd lBound(lnum),uBound(unum);
        lBound.setZero(lnum);
        uBound.setZero(unum);

    bnum = 0;
    int l = 0;
    int u = 0;
    for (int i = 0; i<n;i++){
        if (status.at(i)==1) {
            U.col(u) = constraints.col(i);
            uBound(u) = upper(i);
            u++;}
        else if (status.at(i)==-1) {
            L.col(l) = constraints.col(i);
            lBound(l) = lower(i);
            l++;
        }
        else {
            basis.col(bnum) = constraints.col(i);    // creating Basis matrix
            bnum++;
        }
    }

        VectorXd C_bar(n);
          C_bar= (c.transpose() - C_basis.transpose() * basis.inverse() * constraints);  // c bar transpose is calculated


        //Validating the invariant of dual simplex method
        int optimality = 0;
        for (int i=0;i<n;i++){
            if (status.at(i)==-1 && (C_bar(i)<-1*epsilon)){printf("Ratio Test is incorrecct");optimality++;}     // c_j>-eps and c_j<eps
            else if (status.at(i)==1 && (C_bar(i)>epsilon)){printf("Ratio Test is incorrect");optimality++;}
        }


        basicSolution = basis.inverse() * (b - L * lBound - U * uBound);  //basic Solution is computed


        bool toUpper;
        int counter = 0;
        int found = -1;
        int counterfound;

        // Verifying primal feasibility condition(i.e checking if basic variables are not violating their respective bounds)
    for (int i = 0; i < n; i++){
        if (!status.at(i)) {
            if (basicSolution(counter)<lower(i)-epsilon){
                found = i;
                counterfound = counter;
                toUpper = false;
            }
            else if (basicSolution(counter)>upper(i)+epsilon){
                found = i;
                counterfound = counter;
                toUpper = true;
            }
            counter++;
        }
    }


    if (found==-1) {         // Optimal solution is reached
        myfile<<"optimal"<<endl;
        for (int i =0;i<status.size();i++){
            myfile<<status.at(i)<<" ";
        }
        myfile<<endl;

        break;
    }



        VectorXd row(n);
        row = basis.inverse().row(counterfound) * constraints;

// Ratio test is performed for determining entering variable -------------------
        double min = 1000000000;
        int index = 0;
    for (int k = 0 ; k < n;k++){
        if (status.at(k)==-1){
            if (!toUpper){
                if(row(k)<-1*epsilon) {
                    if (abs(C_bar(k)/row(k)) < min) {min = abs(C_bar(k)/row(k)); index = k;}
                }
        }
            else if (toUpper){
                if(row(k)>epsilon) {
                    if (abs(C_bar(k)/row(k)) < min) {min = abs(C_bar(k)/row(k)); index = k;}
                }
            }
        }
        else if (status.at(k)==1){
            if (!toUpper){
                if(row(k)>epsilon) {
                    if (abs(C_bar(k)/row(k)) < min) {min = abs(C_bar(k)/row(k)); index = k;}
                }
            }
            else if (toUpper){
                if(row(k)<-1*epsilon) {
                    if (abs(C_bar(k)/row(k)) < min) {min = abs(C_bar(k)/row(k)); index = k;}
                }
            }
        }
    }

        //--------------------------------


    if (min==1000000000) {   // Infeasibility condition is reached

            myfile<<"infeasible"<<endl;
            for (int i =0;i<status.size();i++){
                myfile<<status.at(i)<<" ";
            }
        myfile<<endl;
             break;
        }                   //-------------------------


    for (int i=0;i<n;i++){      // Column statuses are updated accordingly
        if (i==found) {
            if (toUpper) status.at(i) = 1;
            else status.at(i) = -1;
        }
        else if (i==index) {
            status.at(i) = 0;
        }
    }                           //----------------------------------


    }
            myfile.close();
            return  0;
    }