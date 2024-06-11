#include <iostream>
#include <string>
#include <fstream>
#include<cmath>
using namespace std;
struct Var_INIT {float x, y, xx, yy, xy;};
class Administrador{
    private:
    Var_INIT *db = NULL;
    int tam;
    float correlacion = 0, pendiente_m = 0, parametro_b = 0;
    float sum_x = 0, sum_y = 0, sum_xx = 0, sum_yy = 0, sum_xy = 0;
    public:
    Administrador() : db(NULL), tam(0),correlacion(0),pendiente_m(0),parametro_b(0) {}
    ~Administrador() { delete[] db; }
    void convertir_coma_punto(string &linea){
        int tam_lin=linea.size();
        for(int j=0;j<tam_lin-1;j++)
        {
            if(linea[j] ==','){linea[j] = '.';}
            
        }
        }
    void importar() {
        string linea;
        int N = 0;
        ifstream leerArchivo("data/mmc.csv");
        while (getline(leerArchivo, linea)) {
            if (linea.size() != 0) {N++;}
        }
        leerArchivo.close();
        leerArchivo.open("data/mmc.csv");
        delete[]db;
        tam = N;
        db = new Var_INIT[tam];
        for (int i = 0; i < N; i++) {
            getline(leerArchivo, linea, ';');
            convertir_coma_punto(linea);
            db[i].x = stof(linea.c_str());
            db[i].xx = pow(db[i].x,2);
            getline(leerArchivo, linea);
            convertir_coma_punto(linea);
            db[i].y = stof(linea.c_str());
            db[i].yy = pow(db[i].y,2);
            db[i].xy = db[i].x * db[i].y;
            sum_x+=db[i].x;
            sum_y+=db[i].y;
            sum_xx+=db[i].xx;
            sum_yy+=db[i].yy;
            sum_xy+=db[i].xy;
        }
        leerArchivo.close();
    }
    void exportar_cvs(){
        ofstream escribirArchivo("result/process.csv", ios::trunc);
        bool firstTime=true;
        escribirArchivo << "X" << ";" << "Y" << ";" << "XX" << ";" << "YY" << ";" << "XY" << ";" << "S:X"<< ";" <<"S:Y"<< ";" <<"S:XX"<< ";" <<"S:YY"<< ";" <<"S:XY"<<endl;
        for (int i = 0; i < tam; i++) {
            escribirArchivo << db[i].x << ";" << db[i].y << ";" << db[i].xx << ";" << db[i].yy << ";" << db[i].xy;
        if (firstTime==true){escribirArchivo <<";"<<sum_x<< ";" <<sum_y<< ";" <<sum_xx<< ";" <<sum_yy<< ";" <<sum_xy; firstTime=false;}
        escribirArchivo<<endl;
        }
        escribirArchivo.close();
    }
    void exportar_txt(){
        ofstream escribirArchivo("result/result.txt", ios::trunc);
        escribirArchivo << "Para determinar los resultados del metodo de minimos cuadrados, se utilizan las siguientes ecuaciones:"<<endl;
        escribirArchivo <<"Ecuaciones de la correlacion: "<<endl;
        escribirArchivo<<"\\[p_{correlacion} = \\frac{{(N)(\\sum xy) - (\\sum x)(\\sum y)}}{{\\sqrt{{[(N)(\\sum x^2) - (\\sum x)^2][(N)(\\sum y^2) - (\\sum y)^2]}}}}\\] "<<endl;
        escribirArchivo<<"\\[\\frac{{("<<tam<<")\\("<<sum_xy<<") - \\("<<sum_x<<")\\("<<sum_y<<")}}{{\\sqrt{{[("<<tam<<")\\("<<sum_xx<<") - \\("<<sum_x<<")^2][("<<tam<<")\\("<<sum_yy<<") - (\\"<<sum_y<<")^2]}}}}\\]= "<<correlacion<<endl;
        escribirArchivo <<"Ecuaciones de la pendiente:"<<endl; 
        escribirArchivo<<"\\[m_{pendiente} = \\frac{{(N)(\\sum xy) - (\\sum x) (\\sum y)}}{{(N)(\\sum x^2) - (\\sum x)^2}}\\] "<<endl; 
        escribirArchivo<<"\\[\\frac{{("<<tam<<")\\("<<sum_xy<<") - \\("<<sum_x<<") \\("<<sum_y<<")}}{{("<<tam<<")\\("<<sum_xx<<")- \\("<<sum_x<<")^2}}\\] = "<<pendiente_m<<endl;
        escribirArchivo<<"Ecuaciones del parametro b:"<<endl;
        escribirArchivo<<"\\[b_{parametro} = \\frac{{(\\sum y)(\\sum x^2) - (\\sum x)(\\sum xy)}}{{(N)(\\sum x^2) - (\\sum x)^2}}\\]"<<endl;
        escribirArchivo<<"\\[b_{parametro} = \\frac{{\\("<<sum_y<<")\\("<<sum_xx<<") - \\("<<sum_x<<")\\("<<sum_xy<<")}}{{("<<tam<<")\\("<<sum_xx<<") - \\("<<sum_x<<")^2}}\\] = "<<parametro_b<<endl;
        escribirArchivo << "Con una correlacion de " << correlacion << ". El metodo de regresion " << (abs(correlacion) > 0.5 ? "es recomendable" : "no es recomendable") <<endl;
        escribirArchivo<<"Con los datos adquiridos y aplicando las ecuaciones numeradas, la recta obtenida seria: \\[y="<<pendiente_m<<"x"<<((parametro_b) >= 0 ? "+" : "") <<parametro_b<<"\\]."<<endl;
    }
    void mmc(){
        correlacion=(((tam * sum_xy) - (sum_x * sum_y))/(sqrt(tam * sum_xx - sum_x * sum_x)*sqrt(tam * sum_yy - sum_y * sum_y)));
        pendiente_m=((tam * sum_xy) - (sum_x * sum_y)) / ((tam * sum_xx) - (sum_x * sum_x));
        parametro_b=((sum_y * sum_xx) - (sum_x * sum_xy)) / ((tam * sum_xx) - (sum_x * sum_x));
    }
    void exportar_grafic() {
        ofstream pyFile("result/plot_data.py", ios::trunc);
        pyFile << "import matplotlib.pyplot as plt\n";
        pyFile << "x = [";for (int i = 0; i < tam; i++){pyFile << db[i].x;if (i < tam - 1) pyFile << ", ";}
        pyFile << "]\n";
        pyFile << "y = [";for (int i = 0; i < tam; i++){pyFile << db[i].y;if (i < tam - 1) pyFile << ", ";}
        pyFile << "]\n";
        pyFile << "plt.scatter(x, y)\n";
        pyFile << "plt.plot(x, ["<<pendiente_m<<" * xi + "<<parametro_b<<" for xi in x], color='red')\n";
        pyFile << "plt.xlabel('X')\n";
        pyFile << "plt.ylabel('Y')\n";
        pyFile << "plt.title('Grafico de X vs Y')\n";
        pyFile << "plt.savefig('result/Graf_MMC.png', bbox_inches='tight', dpi=300)\n";
        pyFile.close();
        system("python result/plot_data.py");
    }
};
class Menu{
	public:
		void mostrar_menu(Administrador &punt){
			string opcion="";bool c;
				do{
					system("cls");
					cout<<"__________________________________________________"<<endl;
                    cout<<"|      Generador del metodo minimos cuadrados     |"<<endl;
                    cout<<"|_________________________________________________|"<<endl;
					cout<<"\t 1) Iniciar MMC"<<endl;
					cout<<"\t 2) Resetear datos"<<endl;
					cout<<"\t 3) Salir"<<endl;
					cout<<"Ingrese opcion (1-3): ";cin>>opcion;
					cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
					if(opcion.length()==1 and isdigit(opcion[0]) and ((opcion >="1") and (opcion <= "3"))){c=false; break;}
					else c = true;
				}while(c==true);
				char op = opcion[0];
					switch(op){	
					case '1':
                        punt.importar();
                        punt.mmc();
                        punt.exportar_cvs();
                        punt.exportar_txt();
                        punt.exportar_grafic();
                        remove("result/plot_data.py");
                        cout<<"Proceso del MMC completado, datos afiliados en result/"<<endl;
						break;
					case '2':
                            remove("result/process.csv");
                            remove("result/result.txt");
                            remove("result/Graf_MMC.png");
						break;
					case '3':
                    punt.~Administrador();
                    return;
						break;
				}
		}
};
int main(){
    Menu menu;
    Administrador obj;
    menu.mostrar_menu(obj);
    obj.~Administrador();
    return 0;
}
