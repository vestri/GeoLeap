/** Copyright 2015 Christophe Vestri
*
* Convert Geo WGS84 to Carto Lambert93
*/

#include <iostream>
using namespace std;

const double PI = std::atan(1.0) * 4;

void geo2carto(const double &latitude, const double &longitude, double &x, double &y)
{
    // computation 
    // see http://www.forumsig.org/showthread.php/3239-Conversion-Lat-Long-vers-Lambert-93?p=314344&viewfull=1#post314344

    //variables:
    double deg2rad = PI / 180;
    double a = 6378137; //demi grand axe de l'ellipsoide (m)
    double e = 0.08181919106; //première excentricité de l'ellipsoide
    double lc = deg2rad * 3;
    double l0 = deg2rad * 3;
    double phi0 = deg2rad*46.5; //latitude d'origine en radian
    double phi1 = deg2rad * 44; //1er parallele automécoïque
    double phi2 = deg2rad * 49; //2eme parallele automécoïque

    double x0 = 700000; //coordonnées à l'origine
    double y0 = 6600000; //coordonnées à l'origine

    double phi = deg2rad*latitude;
    double l = deg2rad*longitude;

    //calcul des grandes normales
    double gN1 = a / sqrt(1 - e * e * sin(phi1) * sin(phi1));
    double gN2 = a / sqrt(1 - e * e * sin(phi2) * sin(phi2));

    const double pi = 3.14159265358979323846;
    //calculs des latitudes isométriques
    double gl1 = log(tan(pi / 4 + phi1 / 2) * pow((1 - e * sin(phi1)) / (1 + e * sin(phi1)), e / 2));
    double gl2 = log(tan(pi / 4 + phi2 / 2) * pow((1 - e * sin(phi2)) / (1 + e * sin(phi2)), e / 2));
    double gl0 = log(tan(pi / 4 + phi0 / 2) * pow((1 - e * sin(phi0)) / (1 + e * sin(phi0)), e / 2));
    double gl = log(tan(pi / 4 + phi / 2) * pow((1 - e * sin(phi)) / (1 + e * sin(phi)), e / 2));

    //calcul de l'exposant de la projection
    double n = (log((gN2 * cos(phi2)) / (gN1 * cos(phi1)))) / (gl1 - gl2);//ok

    //calcul de la constante de projection
    double c = ((gN1 * cos(phi1)) / n) * exp(n * gl1);//ok

    //calcul des coordonnées
    double ys = y0 + c * exp(-1 * n * gl0);

    x = x0 + c * exp(-1 * n * gl) * sin(n * (l - lc));
    y = ys - c * exp(-1 * n * gl) * cos(n * (l - lc));
}


int main(int argc, char **argv)
{
    // variables
    double latitude, longitude, x, y;

    if (argc < 3) {
        cout << "Usage:\n geo2carto lat long\n";
        return 1;
    }

    latitude = atof(argv[1]);
    longitude = atof(argv[2]);

    geo2carto(latitude, longitude, x, y);

    cout.precision(25);
    cout << "X: " << x << " Y: " << y << endl;

    return 0;
}
