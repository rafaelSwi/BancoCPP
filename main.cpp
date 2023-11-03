#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>
using namespace std;

class Evento {
public:
    string tipo;
    int min;
    int qtde;
};

void limparTela(int howMuch) {
    for (int i=0; i < howMuch; i++) {
        cout << "\n" << endl;
    }
}

struct nodo {
    nodo *elo;
    string info;
};

struct fila {
    nodo *frente;
    nodo *re;
    int tamanho;
};

void inicializar(fila *f) {
    f->frente = NULL;
    f->re = NULL;
    f->tamanho = 0;
}

void exibirTodos (fila *f) {
    
    cout << "[*] COMANDO: EXIBIR A FILA\n" << endl;
    
    if (f->tamanho == 0) {
        cout << "[!] A fila esta vazia.\n" << endl;
        return;
    }
    
    nodo *index;
    index = f->frente;
    for (int i=0; i < f->tamanho; i++) {
        cout << (i+1) << ". " << index->info << "\n" << endl;
        if (index->elo == NULL) {
            return;
        } else {
            index = index->elo;
        }
    }
    
    cout << "[*] FIM DA FILA.\n" << endl;
    return;
    
}

void adicionar(fila *f, string info) {
    
    nodo *novo;
    novo = new (nothrow) nodo;
    novo->info = info;
    novo->elo = NULL;
    
    if (f->frente == NULL) {
        cout << "[+] '" << info << "' entrou na fila vazia.\n" << endl;
        f->frente = novo;
        f->re = novo;
        f->tamanho++;
        return;
    } else {
        cout << "[+] '" << info << "' entrou na fila.\n" << endl;
        if (f->re != NULL) {
            f->re->elo = novo;
        }
        f->re = novo;
        f->tamanho++;
        return;
    }
    
}

void removerPrimeiro(fila *f) {
    
    if (f->frente == NULL || f->tamanho <= 0) {
        cout << "[!] A fila esta vazia, nada tem como remover ninguem.\n" << endl;
        return;
    }
    
    if (f->frente->elo != NULL) {
        f->frente = f->frente->elo;
        cout << "[!] O Primeiro da fila foi removido.\n" << endl;
        return;
    }
    
    cout << "[!] Ninguem saiu da fila.\n" << endl;
    return;
}

void criarEventos() {
    
    int minutos;
    
    Evento eventos[100] = {};
    
    for (int i=0; i < 100; i++) {
        eventos[i].min = 0;
        eventos[i].qtde = 0;
        eventos[i].tipo = "";
    }
    
    cout << "[>] Insira a quantidade de minutos:" << endl;
    cin >> minutos;
    
    limparTela(150);
    
    while (true) {
        
        Evento evento;
        evento.min = 1;
        evento.qtde = 0;
        evento.tipo = "";
        
        for (int i = 0; i < 100; i++) {
            if (eventos[i].tipo == "") {
                evento.min = i;
                break;
            }
        }
        
        cout << "[*] Minuto de ocorrencia do evento: " << (evento.min+1) << "\n\n" << endl;
        
        cout << "[>] Insira o Tipo do Evento ( (F)isico , (J)urifica, (N)inguem ):" << endl;
        cin >> evento.tipo;
        string tp = evento.tipo;
        
        if (tp[0] != 'F' && tp[0] != 'J' && tp[0] != 'N') {
            cout << "[!] Input '" << tp << "' invalido. Atribuindo (N)inguem." << endl;
            evento.tipo = "N";
        }
        
        if (evento.tipo != "N") {
            cout << "[>] Insira a quantidade de documentos (1...10)." << endl;
            cin >> evento.qtde;
            
            if (evento.qtde < 1 || evento.qtde > 10) {
                cout << "[!] Valor '" << evento.min << "' invalido. Atribuindo '1'.\n" << endl;
                evento.qtde = 1;
            }
            
        }
        
        this_thread::sleep_for(chrono::seconds(2));
        limparTela(150);
        
        cout << "\n[*] Minuto de Ocorrencia: " << (evento.min+1) << endl;
        cout << "\n[*] Tipo: " << evento.tipo << endl;
        cout << "\n[*] Quantidade de Documentos: " << evento.qtde << "\n\n" << endl;
        
        cout << "[!] Digite '0' para salvar este evento e continuar inserindo.\n" << endl;
        cout << "[!] Digite '1' para salvar este evento e parar de inserir.\n" << endl;
        cout << "[!] Digite '2' para re-fazer este evento.\n" << endl;
        int opcao = 0;
        cin >> opcao;
        
        limparTela(150);
        
        if (opcao == 0) {
            for (int i = 0; i < 100; i++) {
                if (eventos[i].tipo == "") {
                    eventos[i].min = evento.min;
                    eventos[i].qtde = evento.qtde;
                    eventos[i].tipo = evento.tipo;
                    i = 1000;
                }
            }
            limparTela(150);
            continue;
        }
        
        if (opcao == 1) {
            for (int i = 0; i < 100; i++) {
                if (eventos[i].tipo == "") {
                    eventos[i].min = evento.min;
                    eventos[i].qtde = evento.qtde;
                    eventos[i].tipo = evento.tipo;
                    i = 1000;
                }
            }
            limparTela(150);
            break;
        }
        
        if (opcao == 2) {
            continue;
        }
        
        continue;
        
    }
    
}

void gerarSaida() {
    
    
    
}

int main() {
    
    criarEventos();
    
    fila fila_teste;
    inicializar(&fila_teste);
    
    // Adiciona diversos nomes na fila
    adicionar(&fila_teste, "Rafael");
    adicionar(&fila_teste, "Bernardo");
    adicionar(&fila_teste, "Enzo");
    
    exibirTodos(&fila_teste);
    
    removerPrimeiro(&fila_teste);
    
    exibirTodos(&fila_teste);
    
    cout << "\n" << endl;
    
}


