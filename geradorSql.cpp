#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

string templateSql = "INSERT INTO principal.curso_estrutura (semestre, disciplina, carga_horaria, descricao, curso) VALUES\n";
bool firstLoop = true;
string inputFilename = "cursos.csv";
string outputFilename = "insert.txt";

// Definindo a estrutura para armazenar informações sobre um curso
struct Disciplina {
    int semestre;
    string disciplina;
    int carga_horaria;
    int curso;
    string descricao;
};


void excluirUltimoCaractere(const string& inputFilename) {
    ifstream arquivoIn(inputFilename);

    // Lendo todo o conteúdo do arquivo
    string conteudo;
    char caractere;
    while (arquivoIn.get(caractere)) {
        conteudo += caractere;
    }

    arquivoIn.close();

    // Se o arquivo estiver vazio, não há nada para fazer
    if (conteudo.empty()) {
        cout << "O arquivo está vazio." << endl;
        return;
    }

    // Removendo os últimos caractere
    conteudo.pop_back();
    conteudo.pop_back();


    // Abrindo o arquivo em modo de escrita para truncá-lo
    ofstream arquivoOut(inputFilename, ofstream::trunc);
    if (!arquivoOut.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    // Escrevendo o conteúdo modificado de volta no arquivo
    arquivoOut << conteudo;
    arquivoOut.close();
}


// Processa uma linha do arquivo CSV e extrair os itens
vector<string> extrairItens(const string& linha) {
    stringstream ss(linha);
    string item;
    vector<string> itens;
    while (getline(ss, item, ';')) {
        itens.push_back(item);
    }
    return itens;
}

// Processa e insere informações de curso no banco de dados
void inserirDisciplina(const Disciplina& curso) {
	
    // Convertendo os valores não-string para string
    string semestre_str = to_string(curso.semestre);
    string disciplina_str = curso.disciplina;
    string carga_horaria_str = to_string(curso.carga_horaria);
    string curso_str = to_string(curso.curso);
    string descricao_str = curso.descricao;

    // Concatenando as strings
    string linha = "\t("+ semestre_str + ", '" + disciplina_str + "', " + carga_horaria_str + ", '" + descricao_str + "', "  + curso_str + "),";
							
	ofstream myfile;
	myfile.open ("insert.txt", ios::app);
	
	if(firstLoop) {
		myfile << templateSql << linha << "\n";
		firstLoop = false;
	} else {
		myfile << linha << "\n";
	}
	
	
	myfile.close();
  
}

// Lê o arquivo CSV de cursos e processar suas linhas
void gerarSql(const string& inputFilename) {
    ifstream arquivo(inputFilename);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha)) {
        vector<string> itens = extrairItens(linha);
        if (itens.size() == 5) {
            Disciplina disciplina;
            disciplina.semestre = stoi(itens[0]);
            disciplina.disciplina = itens[1];
            disciplina.carga_horaria = stoi(itens[2]);
            disciplina.descricao = itens[3];
            disciplina.curso = stoi(itens[4]);

            inserirDisciplina(disciplina);
        } else {
            cerr << "Formato inválido da linha: " << linha << endl;
        }
    }

    arquivo.close();
    
    excluirUltimoCaractere(outputFilename);
}

int main() {
    setlocale(LC_ALL, "");

	ofstream myfile;
	myfile.open (outputFilename);    

    gerarSql(inputFilename);
	    
	cout<<"Fim da execução!";
	return 0;
}

