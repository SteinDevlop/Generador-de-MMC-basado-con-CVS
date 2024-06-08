#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
using namespace std;
struct Var_INIT{
    int x,y,xx,yy,xy;
};
class Administrador{
    private:
    Var_INIT *db = NULL;
    int tam;
    float correlacion, pendiente_m, parametro_b,sum_x,sum_y,sum_xx,sum_yy,sum_xy;
    public:
    Administrador() : db(NULL), tam(0),correlacion(0),pendiente_m(0),parametro_b(0) {}
    void importar(){
        string linea;
        int n = -1;
        ifstream leerArchivo("data/mmc.csv");
        while (getline(leerArchivo, linea)) {
            if (linea.size() != 0) {
                n++;
            }
        }
        leerArchivo.close();
        leerArchivo.open("database.txt");
        tam = n;
        db = new Var_INIT[tam];
        for (int i = 1; i < n; i++) {
            getline(leerArchivo, linea, ';');
            db[i].x = atof(linea.c_str());
            db[i].xx = pow(db[i].x,2);
            getline(leerArchivo, linea);
            db[i].y = atof(linea.c_str());
            db[i].yy = pow(db[i].y,2);
            db[i].xy = db[i].x * db[i].y;
            sum_x+=db[i].x;
            sum_y+=db[i].y;
            sum_xx+=db[i].xx;
            sum_yy+=db[i].yy;
            sum_xy+=db[i].xy;
        }
    }
    void exportar_cvs(){
        ofstream escribirArchivo("result/process.csv", ios::trunc);
        bool firstTime=true;
        escribirArchivo << "X" << ";" << "Y" << ";" << "XX" << ";" << "YY" << ";" << "XY" << ";" << "S:X"<< ";" <<"S:Y"<< ";" <<"S:XX"<< ";" <<"S:YY"<< ";" <<"S:XY"<<endl;
        for (int i = 0; i < tam; i++) {
            escribirArchivo << db[i].x << ";" << db[i].y << ";" << db[i].xx << ";" << db[i].yy << ";" << db[i].xy;
        if (firstTime==true){escribirArchivo <<";"<<sum_x<< ";" <<sum_y<< ";" <<sum_xx<< ";" <<sum_yy<< ";" <<sum_xy; firstTime=false;}
        else if(firstTime==false){escribirArchivo<<endl;}
        }
        escribirArchivo.close();
    }
    void exportar_txt(){
        ofstream escribirArchivo("result/result.txt", ios::trunc);
        escribirArchivo << "Para determinar los resultados del metodo de minimos cuadrados, se utilizan las siguientes ecuaciones:"<<endl;
        escribirArchivo <<"Ecuacion de la correlacion: \\[p_{correlacion} = \\frac{{N\\sum xy - (\\sum x)(\\sum y)}}{{\\sqrt{{[N\\sum x^2 - (\\sum x)^2][N\\sum y^2 - (\\sum y)^2]}}}}\\] => \\[\\frac{{"<<tam<<"\\"<<sum_xy<<" - (\\"<<sum_x<<")(\\"<<sum_y<<")}}{{\\sqrt{{["<<tam<<"\\"<<sum_x<<"^2 - (\\"<<sum_x<<")^2]["<<tam<<"\\"<<sum_y<<"^2 - (\\"<<sum_y<<")^2]}}}}\\]= "<<correlacion<<endl;
        escribirArchivo <<"Ecuacion de la pendiente: \\[m_{pendiente} = \\frac{{n\\sum xy - \\sum x \\sum y}}{{n\\sum x^2 - (\\sum x)^2}}\\] => \\[\\frac{{"<<tam<<"\\"<<sum_xy<<" - \\"<<sum_x<<" \\"<<sum_y<<"}}{{"<<tam<<"\\"<<sum_x<<"^2 - (\\"<<sum_x<<")^2}}\\] = "<<pendiente_m<<endl;
        escribirArchivo<<"Ecuacion del parametro b: \\[b_{parametro} = \\frac{{\\sum y - a \\sum x}}{{n}}\\] =>\\[\\[b_{parametro} = \\frac{{\\"<<sum_y<<" - a \\"<<sum_x<<"}}{{"<<tam<<"}}\\] = "<<parametro_b<<endl;
        escribirArchivo << "Con una correlacion de " << correlacion << ". El metodo de regresion " << (correlacion > 0.5 ? "es recomendable" : "no es recomendable") <<endl;
        escribirArchivo<<"Con los datos adquiridos y aplicando las ecuaciones numeradas, la recta obtenida seria: \\[y="<<pendiente_m<<"x+"<<parametro_b<<"\\]."<<endl;
    }
    void mmc(){
        correlacion=((tam * sum_xy) - (sum_x * sum_y)) / (pow((tam * sum_xx - sum_x * sum_x),0.5) *pow((tam * sum_yy - sum_y * sum_y),0.5));
        pendiente_m=((tam * sum_xy) - (sum_x * sum_y)) / ((tam * sum_xx) - (sum_x * sum_x));
        parametro_b=((sum_y * sum_xx) - (sum_x * sum_xy)) / ((tam * sum_xx) - (sum_x * sum_x));
    }
};
int main(){
    return 0;
}