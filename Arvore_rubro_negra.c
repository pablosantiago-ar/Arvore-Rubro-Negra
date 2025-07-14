#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BLACK 0
#define RED 1
#include <assert.h> // Função para verificar se a arvore é verdadeira

#define BLACK 0
#define RED 1

typedef struct tagNode {
    int size;
    int color; //Adicionei para que haja a mudança de cores, quando houver o balanceamento conseguir fazer a mudança
    char *data;
    int level;
    struct tagNode *parent;
    struct tagNode *left;
    struct tagNode *right;
} TNode;

typedef struct tagBinaryTree {
    int numberNodes;
    int dataSize;
    TNode *root;
    int deepth;
    int (*comparacao)(char *data1, char *data2);
} TBinaryTree;


bool TNode_create(TNode **node,char *data, int size,int level, TNode *parent, int cor); //preciso adicionar o parametro de cor nessa função
void TNode_destroy(TNode *node);
void TNode_setParent(TNode *node,TNode *parent);
TNode *TNode_getParent(TNode *node);
void TNode_setLeft(TNode *node, TNode *left);
TNode *TNode_qetLeft(TNode *node);
void TNode_setRight(TNode *node, TNode *right);
TNode *TNode_qetRight(TNode *node);
void TNode_setLevel(TNode *node, int level);
int TNode_qetLevel(TNode *node);
char *TNode_getData(TNode *node);
int TNode_getSize(TNode *node);
void TNode_show(TNode *node);
bool TNode_hasChildren(TNode *node);
void TNode_trocaCor(TNode *node);

void rotacaoDir(TNode **node);
void rotacaoEsq(TNode **node);

bool TBinaryTree_create(TBinaryTree *tree,char *data,int dataSize,int (*comparacao)(char *data1, char *data2));
void TBinaryTree_destroy(TBinaryTree *tree);
bool TBinaryTree_add(TBinaryTree *tree,char *data);
bool TBinaryTree_addNode(TBinaryTree *tree,TNode *node1, TNode *node2,int level);
void TBinaryTree_dump(TBinaryTree *tree);
void TBinaryTree_show(TBinaryTree *tree, void (*print)(char *data));
bool TBinaryTree_search(TBinaryTree *tree, char *data);
bool TBinaryTree_delete(TBinaryTree *tree, char *data,bool allNodes);

bool TNode_create(TNode **node,char *data, int size,int level,TNode *parent, int cor){
    if(((*node) = malloc(sizeof(TNode))) != NULL) {
        if(((*node)->data = malloc(size)) != NULL) {
            memcpy((*node)->data,data,size);
            (*node)->size = size;
            (*node)->parent = parent;
            (*node)->level  = level;
            (*node)->left = NULL;
            (*node)->right = NULL;
            (*node)->color = cor;
        } else {
            free(*node);
            return false;
        }
    } else{
        return false;
    }
    return true;
}

void TNode_destroy(TNode *node){
    if(node == NULL){ 
        return;
    }
    if(node->left != NULL){
        TNode_destroy(node->left);
    }
    if(node->right != NULL){
        TNode_destroy(node->right);
    }
    free(node->data);
    free(node);
}

bool TNode_hasChildren(TNode *node){
    return (node->left != NULL) || (node->right != NULL);
}

void TNode_setParent(TNode *node,TNode *parent){ node->parent = parent; }
TNode *TNode_getParent(TNode *node){ return node->parent; }
void TNode_setLeft(TNode *node, TNode *left){ node->left = left; }
TNode *TNode_qetLeft(TNode *node){ return node->left; }
void TNode_setRight(TNode *node, TNode *right){ node->right = right; }
TNode *TNode_qetRight(TNode *node){ return node->right; }
void TNode_setLevel(TNode *node, int level){ node->level = level; }
int TNode_qetLevel(TNode *node){ return node->level; }
char *TNode_getData(TNode *node){ return node->data; }
int TNode_getSize(TNode *node){ return node->size; }

void TNode_show(TNode *node){
    printf("Tamanho dado = %d\n", node->size);
    printf("Dado: ");
    for (int i = 0; i < node->size; i++) {
        printf("%02x ", (unsigned char)node->data[i]);
    }
    printf("\nlevel = %d\n", node->level);
    printf("Pai = %p\n", (void*)node->parent);
    printf("Filho left = %p\n", (void*)node->left);
    printf("Filho right = %p\n", (void*)node->right);
    printf("Cor = %d\n", node->color);
}

void TNode_goLevel(TNode *node, int level){
    if(node == NULL){ 
        return;
    }
    if(node->level == level){
        TNode_show(node);
    }
    else {
        TNode_goLevel(node->left, level);
        TNode_goLevel(node->right, level);
    }
}

void TNode_printLevel(TNode *node, int level,void (*print)(char *data)){
    if(node == NULL){ 
        return;
    }
    if(node->level == level){ 
        print(node->data);
        printf(" %d",node->color);
    }
    else {
        TNode_printLevel(node->left,level,print);
        TNode_printLevel(node->right,level,print);
    }
}

void TNode_trocaCor(TNode *node){
    node->color = !node->color;

    if(node->left != NULL){
        node->left->color = !(node->left->color);
    }

    if(node->right != NULL){
        node->right->color = !(node->right->color);
    }
}



void rotacaoDir(TNode **node){ //Faz a rotação a direita
    if (node == NULL || *node == NULL || (*node)->left == NULL) {
        return;
    }

    TNode *node_aux = *node;
    TNode *filho_esq = node_aux->left;

    node_aux->left = filho_esq->right;
    if (filho_esq->right != NULL) {
        filho_esq->right->parent = node_aux;
    }

    filho_esq->parent = node_aux->parent;
    
    // Atualiza o ponteiro do pai para apontar para o novo topo da sub-árvore
    if (node_aux->parent == NULL) {
        // Se node_aux era a raiz, não há o que fazer aqui. O chamador cuidará da raiz da árvore.
    } else if (node_aux == node_aux->parent->right) {
        node_aux->parent->right = filho_esq;
    } else {
        node_aux->parent->left = filho_esq;
    }

    filho_esq->right = node_aux;
    node_aux->parent = filho_esq;
    
    *node = filho_esq; // Atualiza o ponteiro original para o novo topo
    
}

void rotacaoEsq(TNode **node){
if (node == NULL || *node == NULL || (*node)->left == NULL) {
        return;
    }

    TNode *node_aux = *node;
    TNode *filho_dir = node_aux->right;

    node_aux->right = filho_dir->left;
    if (filho_dir->left != NULL) {
        filho_dir->left->parent = node_aux;
    }

    filho_dir->parent = node_aux->parent;
    
    // Atualiza o ponteiro do pai para apontar para o novo topo da sub-árvore
    if (node_aux->parent == NULL) {
        // Se node_aux era a raiz, não há o que fazer aqui. O chamador cuidará da raiz da árvore.
    } else if (node_aux == node_aux->parent->left) {
        node_aux->parent->left = filho_dir;
    } else {
        node_aux->parent->right = filho_dir;
    }

    filho_dir->left = node_aux;
    node_aux->parent = filho_dir;
    
    *node = filho_dir; // Atualiza o ponteiro original para o novo topo
    
}

void DuplaRotacaoEsquerda(TNode **no) {
    rotacaoDir(&((*no)->right));
    rotacaoEsq(no);
}

void DuplaRotacaoDireita(TNode **no) {
    rotacaoEsq(&((*no)->left));
    rotacaoDir(no);
}

bool TBinaryTree_create(TBinaryTree *tree,char *data,int dataSize,int (*comparacao)(char *data1, char *data2)) {
    if(data != NULL && dataSize > 0) {
        if(TNode_create(&(tree->root),data,dataSize,0,NULL, 0)) {
            tree->comparacao = comparacao;
            tree->numberNodes = 1;
            tree->dataSize = dataSize;
            tree->deepth = 0;
            return true;
        }
    }
    return false;
}

void TBinaryTree_destroy(TBinaryTree *tree){
    TNode_destroy(tree->root);
    tree->root = NULL;
    tree->numberNodes = 0;
    tree->dataSize = 0;
    tree->deepth = 0;
}

// Nova função para corrigir a árvore após inserção
void RB_insert_fixup(TBinaryTree *tree, TNode *z) {
    TNode *tio;

    // O loop continua enquanto o pai de 'z' for VERMELHO
    while (z->parent != NULL && z->parent->color == RED) {
        // Pai é filho à esquerda do avô
        if (z->parent == z->parent->parent->left) {
            tio = z->parent->parent->right;
            // CASO 1: Tio é VERMELHO
            if (tio != NULL && tio->color == RED) {
                z->parent->color = BLACK;
                tio->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // Move para o avô e continua a verificação
            } else {
                // CASO 2: Tio é PRETO e 'z' é filho à direita (triângulo)
                if (z == z->parent->right) {
                    z = z->parent;
                    rotacaoEsq(&z); // Rotaciona no pai para transformar em CASO 3
                }
                // CASO 3: Tio é PRETO e 'z' é filho à esquerda (linha)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotacaoDir(&(z->parent->parent));
            }
        } else { // Pai é filho à direita do avô (lógica espelhada)
            tio = z->parent->parent->left;
            // CASO 1: Tio é VERMELHO
            if (tio != NULL && tio->color == RED) {
                z->parent->color = BLACK;
                tio->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // CASO 2: Tio é PRETO e 'z' é filho à esquerda (triângulo)
                if (z == z->parent->left) {
                    z = z->parent;
                    rotacaoDir(&z);
                }
                // CASO 3: Tio é PRETO e 'z' é filho à direita (linha)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotacaoEsq(&(z->parent->parent));
            }
        }
        // Se z chegou na raiz, sai do loop
        if (z == tree->root) {
            break;
        }
    }
    // Garante que a raiz seja sempre PRETA (Propriedade 2)
    tree->root->color = BLACK;
}

bool TBinaryTree_add(TBinaryTree *tree,char *data){
    TNode *node;
    if(data != NULL){
        TNode_create(&node,data,tree->dataSize,0,NULL, 1);
        bool result = TBinaryTree_addNode(tree,tree->root, node,0);
        if(result == false){
            TNode_destroy(node);
            return result;
        }

        if(node->parent == NULL) { // O nó inserido é a raiz
            node->color = BLACK;
            return true;
        }
        if (node->parent->parent == NULL) { // O pai do nó é a raiz
            return true; // Nenhuma violação possível ainda
        }

        RB_insert_fixup(tree, node); // Nova função de correção
        return true;
    
    }
    return false;
}


TNode *avo(TNode *arvore) {
    if((arvore != NULL) && (arvore->parent != NULL)){
        return (arvore->parent)->parent;
    }else{
        return NULL;
    }
}

TNode *tio(TNode *arvore) {
    TNode *NAvo = avo(arvore);
    if (NAvo == NULL){ 
        return NULL;
    }
    if (arvore->parent == NAvo->left){
        return NAvo->right;
    }else{
        return NAvo->left;
    }
}

void consertaRB(TNode **arvore, TNode **node) {
    assert(arvore);

    TNode *NAvo = NULL;
    TNode *NTio = NULL;

    if((*arvore)->parent != NULL) {
        if((*arvore)->parent->color == BLACK) {
            return;
        }
        if((*arvore)->color == RED) {
            NTio = tio(*arvore);
        }
            if (NTio != NULL && NTio->color == RED) {
                NAvo = avo(*arvore);
                (*arvore)->parent->color = BLACK;
                NTio->color = BLACK;
                if (NAvo->parent != NULL) {
                    NAvo->color = RED;
                } 
            } else {
                NAvo = avo(*arvore);
                if (NAvo != NULL) {
                    if ((*arvore)->parent == NAvo->left) { // filho esquerdo
                        if ((*arvore) == (NAvo->left)->left) {
                            if (NAvo->parent != NULL) {
                                if ((NAvo->parent)->left == NAvo)
                                    rotacaoDir(&((NAvo->parent)->left));
                                else
                                    rotacaoDir(&((NAvo->parent)->right));
                            } else {
                                rotacaoDir(node);
                            }


                       } else {
                           if (NAvo->parent != NULL) {
                               if ((NAvo->parent)->left == NAvo) {
                                    DuplaRotacaoDireita(&((NAvo->parent)->left));
                               } else
                                   DuplaRotacaoDireita(&((NAvo->parent)->right));
                           } else { 
                                DuplaRotacaoDireita(node);
                           }
                      }
                   } else { 
                       if ((*arvore) == ((NAvo->right)->right)) {
                           if (NAvo->parent != NULL) {
                               if (((NAvo->parent)->left) == NAvo) {
                                    rotacaoEsq(&((NAvo->parent)->left));
                               } else
                                   rotacaoEsq(&((NAvo->parent)->right));
                           } else {
                                rotacaoEsq(node);
                           }

                       } else {
                           if (NAvo->parent != NULL) {
                               if((NAvo->parent)->left == NAvo) {
                                    DuplaRotacaoEsquerda(&((NAvo->parent)->left));
                               } else {
                                    DuplaRotacaoEsquerda(&((NAvo->parent)->right));
                               }
                           } else {
                                    DuplaRotacaoEsquerda(node);
                           }
                        }
                }
            }
        }
    }
}

bool TBinaryTree_addNode(TBinaryTree *tree,TNode *node1, TNode *node2,int level){
    int result = tree->comparacao(node1->data,node2->data);
    switch(result){
        case 0: 
            return false;
        case 1:
            if(node1->right == NULL){
                node1->right = node2;
                node2->level = level + 1;
                node2->parent = node1;
                tree->numberNodes++;
                if(node2->level > tree->deepth){
                    tree->deepth = node2->level;
                }
                return true;
            } else {
                bool result = TBinaryTree_addNode(tree,node1->right,node2,level+1);
                // consertaRB(&tree->root, &node2);
                RB_insert_fixup(tree, node2);
                return result;
            }
            case -1:
            if(node1->left == NULL){
                node1->left = node2;
                node2->level = level + 1;
                node2->parent = node1;
                tree->numberNodes++;
                if(node2->level > tree->deepth){
                    tree->deepth = node2->level;
                }
                return true;
            } else {
                bool result = TBinaryTree_addNode(tree,node1->left,node2,level+1);
                // consertaRB(&tree->root, &node2);
                RB_insert_fixup(tree, node2);
                return result;
            }
    }
    return false;
}
TNode *buscaNo(TNode *arvore, int k){
    if (arvore == NULL){ 
        return NULL;
    }

    int valor;
    memcpy(&valor, arvore->data, sizeof(int));

    if (valor == k){
        return arvore;
    }

    if (k < valor){
        return buscaNo(arvore->left, k);
    }
    else{
        return buscaNo(arvore->right, k);
    }
}

int filhoEsquerdo(TNode *arvore){
    if(arvore->parent->left == arvore)
        return 1;
    else
        return 0;
}

TNode *irmao(TNode *parent, TNode *noAtual){
    if(parent == NULL){
        return NULL;
    }
    
    if(parent->right->data == noAtual->data){
        return parent->left;
    }else{
        return parent->right;
    }
}

void nBlackIBlackFRed(TNode *aRemover, TNode **arvore){
    assert(arvore);

    TNode *paiARemover = aRemover->parent;

    if(filhoEsquerdo(aRemover) == 1){
        free(aRemover);
        aRemover->parent->left = NULL;
    } else {
        free(aRemover);
        aRemover->parent->right = NULL;
    }

    if(paiARemover->left == NULL){
        if(paiARemover->parent == NULL){
            if((paiARemover->right)->right == NULL){
                rotacaoDir(&((*arvore)->right));
            }
            rotacaoEsq(arvore);
        } 
        else {
            if((paiARemover->right)->right == NULL){
                rotacaoDir(&(paiARemover->right));
            }
            rotacaoEsq(&paiARemover);
        }
    } 
    else {
        if(paiARemover->parent == NULL){
            if((paiARemover->left)->left == NULL){
                rotacaoEsq(&((*arvore)->left));
            }
            rotacaoDir(arvore);
        } else {
            if((paiARemover->right)->right == NULL){
                rotacaoEsq(&(paiARemover->left));
            }
            rotacaoDir(&paiARemover);
        }
    }
    return;
}

void nBlackIRed(TNode *aRemover, TNode **arvore){
    assert(arvore);

    TNode *paiARemover = aRemover->parent;
    
    nBlackIBlackFRed(aRemover,arvore);
    
    paiARemover->color = BLACK;
    if(paiARemover->left != NULL)
        (paiARemover->left)->color = RED;
    if(paiARemover->right != NULL)
        (paiARemover->right)->color = RED;
}

void nBlackIBlackFBlack(TNode *aRemover, TNode **arvore){
    assert(arvore);

    TNode *paiARemover = aRemover->parent;
    TNode *nIrmao = irmao(paiARemover, aRemover);

    nBlackIRed(aRemover, arvore);

    paiARemover->color = BLACK;
    nIrmao->color = RED;
}



TNode **maiorEsq(TNode **pMaiorEsq){
    assert(pMaiorEsq);
    
    if (*pMaiorEsq == NULL){ 
        return NULL;
    }
    if ((*pMaiorEsq)->right == NULL){
        return pMaiorEsq;
    }else{
        return maiorEsq(&((*pMaiorEsq)->right));
    }
}

void TBinaryTree_removeNODE(TNode **arvore, int k){
    assert(arvore);

    if((*arvore) == NULL){ 
        return;
    }
    TNode *aRemover = *arvore;

    aRemover = buscaNo(aRemover, k);

    if(aRemover == NULL){
        return;
    }

    if(aRemover->right == NULL && aRemover->left == NULL){
        if(aRemover->parent == NULL){
            free(aRemover);
            *arvore = NULL;
            return;
        } else {
            if(aRemover->color == RED && aRemover->right == NULL && aRemover->left == NULL){
                if(filhoEsquerdo(aRemover) == 1){
                    free(aRemover);
                    aRemover->parent->left = NULL;
                } else {
                    free(aRemover);
                    aRemover->parent->right = NULL;
                }
                return;
            } else {
                TNode *nIrmao = irmao(aRemover->parent,aRemover);
                    if(nIrmao == NULL){
                        return;
                    }
                if(aRemover->color == BLACK && nIrmao->color == BLACK){
                    if(nIrmao->right == NULL && nIrmao->left == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    } else if(nIrmao->left->color == BLACK && nIrmao->right->color == BLACK){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                    else if(nIrmao->right->color == BLACK && nIrmao->left == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                    else if(nIrmao->left->color == BLACK && nIrmao->right == NULL){
                        nBlackIBlackFBlack(aRemover, arvore);
                        return;
                    }
                } else if(aRemover->color == BLACK && nIrmao->color == BLACK && (nIrmao->left->color == RED || nIrmao->right->color == RED)){
                    if(nIrmao->left != NULL){
                        nBlackIBlackFRed(aRemover, arvore);
                        return;
                    } else if (nIrmao->right != NULL){
                        nBlackIBlackFRed(aRemover, arvore);
                        return;
                    }
                } else if(aRemover->color == BLACK && nIrmao->color == RED){
                    nBlackIRed(aRemover, arvore);
                }
            }

            return;
        }
    } else if (aRemover->right == NULL || aRemover->left == NULL){
        if(aRemover->right != NULL){
            aRemover->data = aRemover->right->data;
            free(aRemover->right);
            aRemover->right = NULL;
        } else {
            aRemover->data = aRemover->left->data;
            free(aRemover->left);
            aRemover->left = NULL;
        }
    } else {
        TNode **nSubstituto = maiorEsq(&(aRemover->left));
        TNode *auxSubstituto = *nSubstituto;

        aRemover->data = (*nSubstituto)->data;
        (*nSubstituto)->parent->left = (*nSubstituto)->left;


        if(aRemover->color == RED){
            if(aRemover->left == NULL && aRemover == aRemover->parent->left && (aRemover->right->right != NULL || aRemover->right->left != NULL)){
                if(aRemover->right->right != NULL){
                    rotacaoDir(&(aRemover->left));
                    rotacaoEsq(&aRemover);
                }
                if(aRemover->right->left != NULL){
                    rotacaoDir(&(aRemover->right));
                    rotacaoEsq(&aRemover);
                    
                }
            } else if(aRemover->left == NULL && aRemover == aRemover->parent->right && (aRemover->left->right != NULL || aRemover->left->left != NULL)){
                if(aRemover->left->right != NULL){
                    rotacaoEsq(&(aRemover->left));
                    rotacaoDir(&aRemover);
                }
                if(aRemover->left->left != NULL){
                    rotacaoEsq(&(aRemover->left));
                    rotacaoDir(&aRemover);
                }
            }

            aRemover->color = BLACK;
            if(aRemover->left != NULL)
                aRemover->left->color = RED;
            if(aRemover->right != NULL)
                aRemover->right->color = RED;
        }
        free(auxSubstituto);
    }
}

void TBinaryTree_dump(TBinaryTree *tree){
    printf("Numero de nos   = %d\n",tree->numberNodes);
    printf("Tamanho do dado = %d\n",tree->dataSize);
    printf("Deepth          = %d\n",tree->deepth);
    printf("Root            = %p\n",(void*)tree->root);
    printf("Comparacao      = %p\n",(void*)tree->comparacao);
    TNode_show(tree->root);
    for(int i=0; i<=tree->deepth; i++){
        printf("Nivel : %d\n",i);
        TNode_goLevel(tree->root,i);
        printf("-----------------------------\n");
    }
}

void TBinaryTree_show(TBinaryTree *tree, void (*print)(char *data)){
    for(int i=0; i<=tree->deepth; i++){
        TNode_printLevel(tree->root,i,print);
        printf("\n");
    }
}

void printNode(char *data){
    printf(" %d", *((int*)data));
}

int compara_int(char *data1,char *data2){
    int a1, a2;
    memcpy(&a1,data1,sizeof(int));
    memcpy(&a2,data2,sizeof(int));
    if(a1 == a2){ 
        return 0;
    }
    return (a1 > a2) ? 1 : -1;
}

int main() {
    TBinaryTree tree;
    int root = 8, data;

    if(TBinaryTree_create(&tree,(char*)&root,sizeof(root),compara_int)){
        printf("Arvore criada com sucesso!\n");
        TBinaryTree_dump(&tree);

        int valores[] = {12, 5, 5, 10, 1, 6, 72, 9, 50, 1};
        for(int i = 0; i < 10; i++){
            data = valores[i];
            if(TBinaryTree_add(&tree,(char*)&data))
                printf("Dado %d adicionado na arvore\n", data);
            else
                printf("Dado %d nao foi adicionado!\n", data);
            TBinaryTree_dump(&tree);
            printf("\n");
        }

        printf("\n");
        TBinaryTree_show(&tree,printNode);
    }

    return 0;
}
