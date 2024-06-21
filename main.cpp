#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iomanip> 
using namespace std;
struct Var_INIT {
    float x, y, xx, yy, xy;
    Var_INIT(): x(0),y(0),xx(0),yy(0),xy(0){}
    };
class Administrador{
    private:
    Var_INIT *db = nullptr;
    int tam;
    float correlacion = 0, pendiente_m = 0, parametro_b = 0;
    float sum_x = 0, sum_y = 0, sum_xx = 0, sum_yy = 0, sum_xy = 0;
    public:
    Administrador() : db(nullptr), tam(0),correlacion(0),pendiente_m(0),parametro_b(0) {}
    ~Administrador() { delete[] db; sum_x = 0, sum_y = 0, sum_xx = 0, sum_yy = 0, sum_xy = 0;}
    void convertir_coma_punto(string &linea){//Funcion por si al exportar los datos los decimales los ponen como , en vez de .
        int tam_lin=linea.size();
        for(int j=0;j<tam_lin-1;j++)
        {
            if(linea[j] ==','){linea[j] = '.';}
            
        }
        }
    void eliminar_BOM(string &linea) {//Elimina el BOM al inicio de mmc.csv que genera problemas con los decimales.
        if (linea.size() >= 3 && (unsigned char)linea[0] == 0xEF && (unsigned char)linea[1] == 0xBB && (unsigned char)linea[2] == 0xBF) {
            linea = linea.substr(3);
        }
    }
    void importar() {//Importa los datos de mmc.csv hacia Var_INIT *db
        sum_x = 0, sum_y = 0, sum_xx = 0, sum_yy = 0, sum_xy = 0;
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
            if (i == 0) {eliminar_BOM(linea);}
            convertir_coma_punto(linea);
            db[i].x = atof(linea.c_str());
            db[i].xx = db[i].x*db[i].x;
            getline(leerArchivo, linea);
            convertir_coma_punto(linea);
            db[i].y = atof(linea.c_str());
            db[i].yy = db[i].y *db[i].y;
            db[i].xy = db[i].x * db[i].y;
            sum_x+=db[i].x;
            sum_y+=db[i].y;
            sum_xx+=db[i].xx;
            sum_yy+=db[i].yy;
            sum_xy+=db[i].xy;
        }
        leerArchivo.close();
    }
    void exportar_cvs(){//Exporta los datos db y las sumatorias a un csv ubicado en result/process.csv
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
    void exportar_txt(){//Exporta en result/result.tex con informacion exportable al estilo Latex
        ofstream escribirArchivo("result/result.tex", ios::trunc);
        escribirArchivo<<"% === No modificar éstos parámetros === "<<endl;
        escribirArchivo<<"\\documentclass[letterpaper, 12pt]{report}\\usepackage[utf8]{inputenc}\\usepackage[english, spanish]{babel}\\usepackage{fullpage}\\usepackage{graphicx}\\usepackage{enumitem} \\usepackage{chngcntr}\\counterwithin{figure}{section}\\renewcommand{\\thesection}{\\arabic{section}} \\renewcommand{\\thesubsection} {\\thesection.\\arabic{subsection}}\\renewcommand{\\baselinestretch}{1.5}\\usepackage{float}"<<endl;
        escribirArchivo<<"% === No modificar éstos parámetros === "<<endl;
        escribirArchivo<<"\\begin{document}"<<endl;
        escribirArchivo << "Para realizar el Mínimo de mínimos cuadrados (MMC), se procede de la siguiente manera:"<<endl;
        escribirArchivo<<"\\begin{enumerate}"<<endl;
        escribirArchivo <<"\\item Determinamos los valores del eje X y el eje Y según se muestra en la tabla \\ref{XeY}."<<endl;
        escribirArchivo<<"\\item A partir de estos valores, calculamos los valores de \\(X^2 , Y^2 , XY\\), los cuales se presentan en la tabla \\ref{XeYeXXeYYeXY}."<<endl;
        escribirArchivo<<"\\item Posteriormente, obtenemos la sumatoria de cada columna de la tabla anterior, generado en la tabla \\ref{T_Sum}"<<endl;
        escribirArchivo<<"\\end{enumerate}"<<endl;
        escribirArchivo<<"\\begin{table}[!ht]"<<endl;
        escribirArchivo<<"\\centering"<<endl;
        escribirArchivo<<"  \\begin{tabular}{|l|l|}"<<endl;
        escribirArchivo<<"\\hline"<<endl;
        escribirArchivo<<" \\textbf{X} & \\textbf{Y} \\\\ \\hline"<<endl;
        for(int i=0;i<tam;i++){escribirArchivo<<db[i].x<<" & "<<db[i].y<<" \\\\ \\hline"<<endl;}
        escribirArchivo<<" \\end{tabular}"<<endl;
        escribirArchivo<<"\\caption{Tabla X e Y}"<<endl;
        escribirArchivo<<"\\label{XeY}"<<endl;
        escribirArchivo<<"\\end{table}"<<endl;
        escribirArchivo<<"\\newpage"<<endl;
        escribirArchivo<<"\\begin{table}[!ht]"<<endl;
        escribirArchivo<<"\\centering"<<endl;
        escribirArchivo<<"  \\begin{tabular}{|l|l|l|l|l|}"<<endl;
        escribirArchivo<<"\\hline"<<endl;
        escribirArchivo<<" \\textbf{X} & \\textbf{Y} & \\textbf{XX} & \\textbf{YY} & \\textbf{XY}\\\\ \\hline"<<endl;
        for(int i=0;i<tam;i++){escribirArchivo<<db[i].x<<" & "<<db[i].y<<" & "<<db[i].xx<<" & "<<db[i].yy<<" & "<<db[i].xy<<" \\\\ \\hline"<<endl;}
        escribirArchivo<<" \\end{tabular}"<<endl;
        escribirArchivo<<"\\caption{Tabla X e Y e XX e YY e XY}"<<endl;
        escribirArchivo<<"\\label{XeYeXXeYYeXY}"<<endl;
        escribirArchivo<<"\\end{table}"<<endl;
        escribirArchivo<<"\\newpage"<<endl;
        escribirArchivo<<"\\begin{table}[!ht]"<<endl;
        escribirArchivo<<"\\centering"<<endl;
        escribirArchivo<<"  \\begin{tabular}{|l|l|l|l|l|}"<<endl;
        escribirArchivo<<"\\hline"<<endl;
        escribirArchivo<<" \\textbf{S:X} & \\textbf{S:Y} & \\textbf{S:XX} & \\textbf{S:YY} & \\textbf{S:XY}\\\\ \\hline"<<endl;
        escribirArchivo<<sum_x<<" & "<<sum_y<<" & "<<sum_xx<<" & "<<sum_yy<<" & "<<sum_xy<<" \\\\ \\hline"<<endl;
        escribirArchivo<<" \\end{tabular}"<<endl;
        escribirArchivo<<"\\caption{Tabla Sumatorias}"<<endl;
        escribirArchivo<<"\\label{T_Sum}"<<endl;
        escribirArchivo<<"\\end{table}"<<endl;
        escribirArchivo<<"\\newpage"<<endl;
        escribirArchivo<<"A partir de los resultados obtenidos, aplicamos las siguientes fórmulas para calcular la correlación, la pendiente y su correspondiente parámetro:"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"\\hfill \\break"<<endl;
        escribirArchivo <<"\\(Correlacion = \\frac{{(N)(\\sum xy) - (\\sum x)(\\sum y)}}{{\\sqrt{{[(N)(\\sum x^2) - (\\sum x)^2][(N)(\\sum y^2) - (\\sum y)^2)}}}} =>\\frac{{("<<tam<<") ("<<sum_xy<<") - ("<<sum_x<<") ("<<sum_y<<")}}{{\\sqrt{{[("<<tam<<") ("<<sum_xx<<") - ("<<sum_x<<")^2][("<<tam<<") ("<<sum_yy<<") - ( "<<sum_y<<")^2]}}}}= "<<fixed<<setprecision(3)<<correlacion<<"\\)"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"\\hfill \\break"<<endl;
        escribirArchivo<<"\\(Pendiente = \\frac{{(N)(\\sum xy) - (\\sum x) (\\sum y)}}{{(N)(\\sum x^2) - (\\sum x)^2}} => \\frac{{("<<tam<<")("<<sum_xy<<") - ("<<sum_x<<") ("<<sum_y<<")}}{{("<<tam<<") ("<<sum_xx<<")- ("<<sum_x<<")^2}}= "<<fixed<<setprecision(3)<<pendiente_m<<"\\)"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"\\hfill \\break"<<endl;
        escribirArchivo<<"\\(Parametro = \\frac{{(\\sum y)(\\sum x^2) - (\\sum x)(\\sum xy)}}{{(N)(\\sum x^2) - (\\sum x)^2}}=>\\frac{{("<<sum_y<<")("<<sum_xx<<") - ("<<sum_x<<")("<<sum_xy<<")}}{{("<<tam<<")("<<sum_xx<<") - ("<<sum_x<<")^2}}= "<<fixed<<setprecision(3)<<parametro_b<<"\\)"<<endl;
        escribirArchivo<<"\\hfill \\break"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"La correlación nos da un valor de " << fixed<<setprecision(3)<<correlacion << ", lo que significa que el método de regresión " << fixed<<setprecision(3)<<(abs(correlacion) > 0.5 ? "es recomendable." : "no es recomendable.") <<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"Con estos datos, determinamos la ecuación de la recta, que sigue la forma \\(y = mx+b\\). Sustituimos los valores obtenidos para determinar la ecuación de la recta específica:"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"\\[y="<<fixed<<setprecision(3)<<pendiente_m<<"x"<<((parametro_b) >= 0 ? "+" : "") <<fixed<<setprecision(3)<<parametro_b<<"\\]"<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"Posteriormente, representamos gráficamente los resultados obtenidos en función de la recta de regresión, con las variables X e Y, se hace referencia en el gráfico \\ref{metodo_mmc}."<<endl;
        escribirArchivo<<""<<endl;
        escribirArchivo<<"\\begin{figure}[h]"<<endl;
        escribirArchivo<<"\\centering"<<endl;
        escribirArchivo<<"\\includegraphics[width=1 \\linewidth]{Graf_MMC.png}"<<endl;
        escribirArchivo<<"\\caption{Gráfico de X vs Y.}"<<endl;
        escribirArchivo<<"\\label{metodo_mmc}"<<endl;
        escribirArchivo<<"\\end{figure}"<<endl;
        escribirArchivo<<"\\end{document}"<<endl;
        escribirArchivo.close();
    }
    void mmc(Administrador &punt){ //Realiza los calculos del proceso del metodo de minimos cuadrados.
        correlacion=(((tam * sum_xy) - (sum_x * sum_y))/(sqrt(tam * sum_xx - sum_x * sum_x)*sqrt(tam * sum_yy - sum_y * sum_y)));
        pendiente_m=((tam * sum_xy) - (sum_x * sum_y)) / ((tam * sum_xx) - (sum_x * sum_x));
        parametro_b=((sum_y * sum_xx) - (sum_x * sum_xy)) / ((tam * sum_xx) - (sum_x * sum_x));
        if(isnan(correlacion) or isnan(pendiente_m) or isnan(parametro_b)){
            cout<<"No es posible generarse .csv, .tex y .png. | Error: mmc:nan."<<endl;
        }   
        else{
            cout<<"Calculos relacionados concluidos ..."<<endl;
            punt.exportar_cvs();
            cout<<"Archivo .csv generado ..."<<endl;
            punt.exportar_txt();
            cout<<"Archivo .tex generado ..."<<endl;
            punt.exportar_grafic();
            remove("result/plot_data.py");
            cout<<"Archivo .png generado ..."<<endl;
            cout<<"Proceso del MMC completado, datos afiliados en result/"<<endl;
        }
    }
    void exportar_grafic() {//Crea el archivo python result/plot_data.py para exportar el .PNG grafico en result/Graf_MMC.png
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
        pyFile << "plt.savefig('result/Graf_MMC.png', bbox_inches='tight', dpi=600)\n";
        pyFile.close();
        system("python result/plot_data.py");
    }
    };
class Menu{//Menu grafico para las opciones
	public:
		void mostrar_menu(Administrador &punt){
			string opcion="";bool c;
            bool salir = false;
            while (!salir) {
				do{
					system("cls");
					cout<<"__________________________________________________"<<endl;
                    cout<<"|   Generador del Metodo de Minimos Cuadrados     |"<<endl;
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
                        system("cls");
                        cout<<"Iniciando proceso MMC ..."<<endl;
                        cout.flush();
                        punt.importar();
                        cout<<"Datos importados ..."<<endl;
                        punt.mmc(punt);
						system("pause"); cout<<"Pulse cualquier tecla para continuar"<<endl;
                        break;
					case '2':
                            system("cls");
                            remove("result/process.csv");
                            remove("result/result.tex");
                            remove("result/Graf_MMC.png");
                            cout<<"Se han limpiado los datos de result/"<<endl;
                            system("pause"); cout<<"Pulse cualquier tecla para continuar"<<endl;
						break;
					case '3':
                        salir = true;
					    break;}}}};
int main(){Menu menu;Administrador obj;menu.mostrar_menu(obj);obj.~Administrador();return 0;}
