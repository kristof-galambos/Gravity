#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

const double PI = 3.1415926535897;
const double G = 6.6740831e-11;

int main()
{
    //read in most important data:
    ifstream in ("in.txt");
    int bodies;
    double T, dt, t;
    in >> bodies >> T >> dt;
    t = 0;
    //store bodies' data here:
    struct Body {
        string name;
        double m, r, x, y, vx, vy, ax, ay;
    } Bodies[bodies];
    //read in bodies' data:
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].name;
        Bodies[i].ax = 0;
        Bodies[i].ay = 0;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].m;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].r;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].x;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].y;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].vx;
    }
    for(int i=0; i<bodies; i++) {
        in >> Bodies[i].vy;
    }
    in.close();

    ofstream out ("out.txt");
    out << "THE RESULTS OF MY GRAVITY SIMULATION\n" <<setprecision(3)<< endl;
    out <<setw(8)<<"Time (s)"<<setw(8)<<"Name"<<setw(12)<<"X (m)"<<setw(12)<<"Y (m)"<<setw(12)<<"Vx (m/s)"<<setw(12)<<"Vy (m/s)"<<endl;
    while (t<T) {
        for(int i=0; i<bodies; i++) {
            Bodies[i].ax = 0;
            Bodies[i].ay = 0;
            for(int j=0; j<bodies; j++) {
                if (i!=j) {
                        double theta;
                        if (Bodies[j].x > Bodies[i].x) {
                            theta = atan((Bodies[j].y-Bodies[i].y)/(Bodies[j].x-Bodies[i].x));
                        } else if (Bodies[j].x < Bodies[i].x) {
                            theta = atan((Bodies[j].y-Bodies[i].y)/(Bodies[j].x-Bodies[i].x))+PI;
                        } else {
                            if (Bodies[j].y > Bodies[i].y) {
                                theta = PI/2;
                            } else {
                                theta = -PI/2;
                            }
                        }
                        double dist_squared = ((Bodies[j].x-Bodies[i].x)*(Bodies[j].x-Bodies[i].x) + (Bodies[j].y-Bodies[i].y)*(Bodies[j].y-Bodies[i].y));
                        double amag = G*Bodies[i].m/dist_squared;
                        if (dist_squared<(Bodies[i].r+Bodies[j].r)*(Bodies[i].r+Bodies[j].r)) {
//                            x[i] = (m[i]*x[i]+m[j]*x[j])/(m[i]+m[j]);
//                            y[i] = (m[i]*y[i]+m[j]*y[j])/(m[i]+m[j]);
//                            vx[i] = (m[i]*vx[i]+m[j]*vx[j])/(m[i]+m[j]);
//                            vy[i] = (m[i]*vy[i]+m[j]*vy[j])/(m[i]+m[j]);
//                            m[i] +=m[j];
//                            r[i] = pow((r[i]*r[i]*r[i]+r[j]*r[j]*r[j]), 1/3.0);
//                            cout <<"Collision at time "<<t<<" between "<<names[i]<<" and "<<names[j]<<" at x="<<x[i]<<" and y="<<y[i]<< endl;
//                            names[i] += names[j];
//                            for(int k=j; k<bodies-1; k++) {
//                                names[k] = names[k+1];
//                                m[k] = m[k+1];
//                                r[k] = m[k+1];
//                                x[k] = x[k+1];
//                                y[k] = y[k+1];
//                                vx[k] = vx[k+1];
//                                vy[k] = vy[k+1];
//                                ax[k] = ax[k+1];
//                                ay[k] = ay[k+1];
//                            }
//                            bodies--;
//                            continue;
                            cout << "Collision!" << endl;
                            return 1;
                        }
                        Bodies[i].ax += amag*cos(theta);
                        Bodies[i].ay += amag*sin(theta);
                }
            }
            Bodies[i].x += Bodies[i].vx*dt + Bodies[i].ax*dt*dt/2;
            Bodies[i].y += Bodies[i].vy*dt + Bodies[i].ay*dt*dt/2;
            Bodies[i].vx += Bodies[i].ax*dt;
            Bodies[i].vy += Bodies[i].ay*dt;
            out <<setw(8)<<t<<setw(8)<<Bodies[i].name<<setw(12)<<Bodies[i].x<<setw(12)<<Bodies[i].y<<setw(12)<<Bodies[i].vx<<setw(12)<<Bodies[i].vy<<endl;
        }
        t += dt;
    }
    out.close();
    cout << "Simulation completed succesfully, see out.txt for the results" << endl;

    return 0;
}
