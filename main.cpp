#include <iostream>
#include <thread>
#include <cctype>
using namespace std;

void limparTela(int howMuch) {
    for (int i=0; i < howMuch; i++) {
        cout << "\n" << endl;
    }
}

struct nodo {
    nodo *elo;
    int qtde;
};

struct fila {
    nodo *frente;
    nodo *re;
    int tamanho;
};

class Caixa {
public:
    bool ocupado;
    int qtdePendentes;
    string tipo;
    fila f;

    void inicializar() {
        ocupado = false;
        qtdePendentes = 0;
        tipo = "";
    }

    int tamanhoDaFila () {
        return f.tamanho;
    }

    void adicionarNaFila(int qtde) {

        if (qtde == 0) {
            return;
        }

        nodo *novo;
        novo = new (nothrow) nodo;
        novo->qtde = qtde;
        novo->elo = NULL;

        if (f.frente == NULL) {
            if (!ocupado) {
                qtdePendentes += qtde;
                ocupado = true;
                return;
            } else {
                f.frente = novo;
                f.re = novo;
                f.tamanho++;
                qtdePendentes += qtde;
                ocupado = true;
                return;
            }
        } else {
            if (f.re != NULL) {
                f.re->elo = novo;
            }
            f.re = novo;
            f.tamanho++;
            qtdePendentes += qtde;
            ocupado = true;
            return;
        }
    }

    void removerPrimeiroDaFila() {

        if (f.frente == NULL || f.tamanho <= 0) {
            return;
        }

        if (f.frente->elo != NULL) {
            f.frente = f.frente->elo;
            ocupado = false;
            f.tamanho--;
            return;
        }

        if (f.frente != NULL) {
            f.frente = NULL;
            ocupado = false;
            f.tamanho--;
            return;
        }

        return;
    }

    int totalDeDocumentos () {

        if (f.tamanho <= 0) {
            return 0;
        }
        int documentos = 0;
        nodo *index;
        index = f.frente;
        for (int i = 0; i < f.tamanho; i++) {
            documentos += index->qtde;
            if (index->elo == NULL) {
                 return documentos;
            } else {
                index = index->elo;
            }
        }
        return 0;
    }

};

class Agencia {
public:
    Caixa caixa1;
    Caixa caixa2;
    int tempoRestante;
    int tempoTotal;

    void gerarLinha(Caixa *c1, Caixa *c2) {
        string output_c1 = (c1->ocupado ? " A " : " L ") + to_string(c1->tamanhoDaFila());
        string output_c2 = (c2->ocupado ? " A " : " L ") + to_string(c2->tamanhoDaFila());
        cout << (tempoTotal-tempoRestante) << output_c1 << output_c2 << "\n";
        //cout << "MINUTO: " << ((tempoTotal-tempoRestante)+1) << " STATUS:"<< (c->ocupado ? " A " : " L ") << " NA FILA: "<<c->tamanhoDaFila() << " QTDE: " << c->qtdePendentes << endl;
    }

    void passarMinuto() {
        tempoRestante--;
    }

    void operarCaixa(Caixa *c) {

        // Quando está no absoluto inicio
        if (c->tamanhoDaFila() == 1 && c->qtdePendentes == 0) {
            c->removerPrimeiroDaFila();
            c->ocupado = true;
            return;
        }

        // Quando você NÃO precisa atender uma pessoa e NÃO tem documentos pendentes
        if (c->tamanhoDaFila() == 0 && c->qtdePendentes <= 0) {
            c->ocupado = false;
            return;
        }

        // Quando você PRECISA atender uma pessoa e NÃO tem documentos pendentes
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes <= 0) {
            c->qtdePendentes += c->f.frente->qtde;
            c->removerPrimeiroDaFila();
            c->ocupado = true;
            return;
        }

        // Quando você PRECISA atender uma pessoa e TEM documentos pendentes
        if (c->tamanhoDaFila() >= 1 && c->qtdePendentes != 0) {
            c->qtdePendentes--;
            return;
        }

        if (c->qtdePendentes > 0) {
            c->qtdePendentes--;
            return;
        }

    }

    void definirTempo(int tempo) {
        tempoRestante = tempo;
        tempoTotal = tempo;
    }


};


int main() {

    fila fila_caixa1;
    fila fila_caixa2;
    Agencia agencia;
    agencia.caixa1.inicializar();
    agencia.caixa2.inicializar();
    agencia.caixa1.f = fila_caixa1;
    agencia.caixa2.f = fila_caixa2;
    agencia.definirTempo(20);

    agencia.caixa1.f.frente = NULL;
    agencia.caixa1.f.re = NULL;
    agencia.caixa1.f.tamanho = 0;
    agencia.caixa2.f.frente = NULL;
    agencia.caixa2.f.re = NULL;
    agencia.caixa2.f.tamanho = 0;

    agencia.caixa1.qtdePendentes = 0;
    agencia.caixa2.adicionarNaFila(4);
    agencia.caixa1.adicionarNaFila(6);
    agencia.caixa2.adicionarNaFila(2);
    //agencia.caixa2.adicionarNaFila(5);
    //agencia.caixa1.adicionarNaFila(0);
    //agencia.caixa1.adicionarNaFila(3);
    //agencia.caixa2.adicionarNaFila(6);
    //agencia.caixa2.adicionarNaFila(2);



    for (int i=0; i<agencia.tempoTotal; i++) {
        agencia.operarCaixa(&agencia.caixa1);
        agencia.operarCaixa(&agencia.caixa2);
        agencia.passarMinuto();
        agencia.gerarLinha(&agencia.caixa1, &agencia.caixa2);
    }


    
}


