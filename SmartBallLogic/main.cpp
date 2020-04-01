//
//  main.cpp
//  SmartBallLogic
//
//  Created by Samuel F Woods on 11/7/19.
//  Copyright © 2019 Samuel F Woods. All rights reserved.
//

/*
    ECEN 403
    main.cpp describes the logic for the Smart Ball Fetch for Team 38 for the Demo
 
    This code starts with a pre-set list of RSSI values that were gathered from real tests.
    The code takes these RSSI values and converts them into a unit for distance.
    Then the code compares these distances to set values ranges and assigns a value range to the data
    The value range will determine whether or not to "move" the Smart Ball using the motors.
    
    This code also includes an algorithm for a psuedo-random movement of the ball.
    This means that the algorithm will ouput a random motor control value.
 */


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>


using namespace std;

int main(int argc, const char * argv[]) {
    
    /* *********************************************************************************************
     
                This block contains the measured RSSI values in signed hex form.
                The values are converted to signed decimal before passing the values
                through the distance estimation block
     
     ***********************************************************************************************/
    
    //Put the values in here
    //each value is in signed form (0xFF)
    vector<string> vect1 = {"D0","C2","B9","B6","B5","B0","AC","AA","B1","B8"};
//    vect1.push_back(0xD8);                              //-40db
//    vect1.push_back(0xD3);                              //-45db
//    vect1.push_back(0xCE);                              //-50db
//    vect1.push_back(0xC9);                              //-55db
//    vect1.push_back(0xC4);                              //-60db
//    vect1.push_back(0xBF);                              //-65db
//    vect1.push_back(0xBA);                              //-70db
//    vect1.push_back(0xB5);                              //-75db
//    vect1.push_back(0xB0);                              //-80db
//    vect1.push_back(0xAB);                              //-85db
//    vect1.push_back(0xA6);                              //-90db
//    vect1.push_back(0xA1);                              //-95db
//    vect1.push_back(0x9C);                              //-100db
    
//    for(int i = 0; i < vect1.size(); i++){
//        cout << vect1.at(i) << endl;
//    }
    
    int y;
    vector<int> vectManip;
    
    for(int i = 0; i < vect1.size(); i++){
        stringstream ss;
        unsigned int x;
        ss << hex << vect1.at(i);
        ss >> x;
        
        if(x > 127){
            y = x - 256;
            //cout << y << "dB" << endl;
            vectManip.push_back(y);
        }
        else{
            //cout << x << "dB" << endl;
            vectManip.push_back(x);
        }
    }
    
    /* *********************************************************************************************
     
                This block contains the logic to convert RSSI into a distance unit
     
     ***********************************************************************************************/
    
    //The formula used is d = 10^( (R1-RSSI) / 20 )
    // d = distance in meters
    // R1 = RSSI at 1 meter (CONSTANT)
    // RSSI = Received Signal Strength Indicator (value in array)
    // 20 = 10 * N, assuming N = 2 (CONSTANT)
    
    float R1;                                //constant RSSI at 1 meter
    float d;                                 //distance in meters
    float RSSI;                              //RSSI variable for the vectManip vector
    bool enableMotors = 0;                   //boolian value that is true when the distance is less than 5ft
    int degShft;                             //number to determine the amount of degrees to shift
    srand(time(NULL));                       //generates a seed from the computer's time for a random number

    //Input test vectors from real data collection with Project_Zero
    //Test vector 1
    vector<int> testVect1 = {-82,-72,-85,-80,-84,-92,-87,-98,-89,-90,-88,-93,-96,-95,-97,-93};  //RSSI values
    vector<int> testDist1 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};                            //Actual distance in feet
    //Test vector 2
    vector<int> testVect2 = {-58,-46,-52,-56,-62,-67,-72,-76,-77,-76,-80};                      //RSSI values
    vector<int> testDist2 = {0,1,2,3,4,5,6,7,8,9,10};                                           //Actual distance in feet
    //Test vector 3
    vector<int> testVect3 = {-56,-53,-45,-48,-55,-63,-70,-74,-65,-62,-57};                      //RSSI values
    vector<int> testDist3 = {0,3,1,2,3,5,7,8,6,5,4};                                            //Actual distance in feet
    //Test vector 4
    vector<int> testVect4 = {-56,-44,-60,-70,-80,-72,-67,-59,-48,-52,-64};                      //RSSI values
    vector<int> testDist4 = {0,1,4,7,10,8,6,4,2,3,5};                                           //Actual distance in feet
    //Test vector 5
    vector<int> testVect5 = {-56,-44,-48,-53,-62,-64,-65,-62,-59,-53,-49};                      //RSSI values
    vector<int> testDist5 = {0,1,2,3,4,5,6,5,4,3,2};                                            //Actual distance in feet
    //Test vector 6
    vector<int> testVect6 = {-57,-48,-54,-49,-57,-62,-71,-75,-73,-70,-70};                      //RSSI values
    vector<int> testDist6 = {0,2,3,2,4,5,7,9,8,7,6};                                            //Actual distance in feet
    
    
    R1 = testVect1.at(0);                                                                       //set RSSI at 1 meter to first element of vector
    
    for (int i = 1; i < testVect1.size(); i++){
        RSSI = testVect1.at(i);                                                                 //take the ith RSSI value from the vector
        d = pow(10.0,( (R1-RSSI) / 20.0 )) * 3.28084;                                           //calculates distance from RSSI value
        cout << "Approximate distance = " << d << " in feet for the RSSI value of " << testVect1.at(i) << "db";
        cout << "\t\tActual distance = " << testDist1.at(i) << " feet." << endl;
        
        if(d < 5.5){                                                                            //if distance is "close enough", motors should turn on
            enableMotors = true;
            cout << "Distance is between 0 and 5 feet. Motors are active.\t\t\t\t\t";
        }
        else if(d > 5.5){                                                                       //else, motors should stay inactive
            enableMotors = false;
            cout << "Distance is greater than 5 feet. Motors are inactive.\n" << endl;
        }
        
    /* *********************************************************************************************
     
                This block contains the logic for the psuedo-random motor movement
     
     ***********************************************************************************************/
    
        
        
        if(enableMotors){
            //A random number is generated using the current time as a seed.
            //The number is then modulated by 7 to make 7 different cases for the degree shift
            degShft = rand() % 7;
            //cout << "degShft is " << degShft << endl;
            
            //Once the connection to the motors is made, the cout's will be replaced with the
            //motor control commands
            switch (degShft){
                case 0: cout << "Motors set to turn left at 90 degrees" << endl;
                    break;
                case 1: cout << "Motors set to turn left at 60 degress" << endl;
                    break;
                case 2: cout << "Motors set to turn left at 30 degrees" << endl;
                    break;
                case 3: cout << "Motors set to continue straight" << endl;
                    break;
                case 4: cout << "Motors set to turn right at 30 degrees" << endl;
                    break;
                case 5: cout << "Motors set to turn right at 60 degrees" << endl;
                    break;
                case 6: cout << "Motors set to turn right at 90 degrees" << endl;
                    break;
                default:    cout << "Something went wrong. Continuing straight" << endl;
                    cout << endl;
            }
            
            cout << endl;
            
        }
        
    }
    
    return 0;       //end of program
}
