#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

bool TNode_create(TNode **node,char *data, int size,int level,TNode *parent, int cor) {
    if(((*node) = malloc(sizeof(TNode))) != NULL) {
        if(((*node)->data = malloc(size)) != NULL) {
            memcpy((*node)->data,data,size);
            (*node)->size = size;
            (*node)->parent = parent;
            (*node)->level  = level;
            (*node)->left = NULL;
            (*node)->right = NULL;
            (*node)->color = RED;
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
    if(node == NULL){
        return;
    }

    if(*node == NULL){
        return;
    }

    TNode *node_aux = *node;
    
    //operações de ponteiro para fazer a troca
    *node = node_aux->left;
    node_aux->left = (*node)->right;
    (*node)->right = node_aux;

    (*node)->parent = node_aux->parent;

    if(node_aux->left != NULL){
        (node_aux->left)->parent = node_aux;
    }

    node_aux->parent = *node;

    //mudança das cores do novo topo para o antigo
    (*node)->color = node_aux->color;
    node_aux->color = RED;

}

bool TBinaryTree_create(TBinaryTree *tree,char *data,int dataSize,int (*comparacao)(char *data1, char *data2)) {
    if(data != NULL && dataSize > 0) {
        if(TNode_create(&(tree->root),data,dataSize,0,NULL,1)) {
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

bool TBinaryTree_add(TBinaryTree *tree,char *data){
    TNode *node;
    if(data != NULL){
        TNode_create(&node,data,tree->dataSize,0,NULL, 1);
        bool result = TBinaryTree_addNode(tree,tree->root, node,0);
        if(result == false){
            TNode_destroy(node);
        }
        return result;
    }
    return false;
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
                if(node2->level > tree->deepth) tree->deepth = node2->level;
                return true;
            } else {
                return TBinaryTree_addNode(tree,node1->right,node2,level+1);
            }
        case -1:
            if(node1->left == NULL){
                node1->left = node2;
                node2->level = level + 1;
                node2->parent = node1;
                tree->numberNodes++;
                if(node2->level > tree->deepth) tree->deepth = node2->level;
                return true;
            } else {
                return TBinaryTree_addNode(tree,node1->left,node2,level+1);
            }
    }
    return false;
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

bool TBinaryTree_search(TBinaryTree *tree, char *data){
    return false;
}

bool TBinaryTree_delete(TBinaryTree *tree, char *data,bool allNodes){
    return false;
}

int compara_int(char *data1,char *data2){
    int a1, a2;
    memcpy(&a1,data1,sizeof(int));
    memcpy(&a2,data2,sizeof(int));
    if(a1 == a2){ 
        return 0;
    }
    return (a2 > a1) ? 1 : -1;
}

int main() {
    TBinaryTree tree;
    int root = 8, data;

    if(TBinaryTree_create(&tree,(char*)&root,sizeof(root),compara_int)){
        printf("Arvore criada com sucesso!\n");
        TBinaryTree_dump(&tree);

        int valores[] = {12, 5, 5, 10};
        for(int i = 0; i < 4; i++){
            data = valores[i];
            if(TBinaryTree_add(&tree,(char*)&data))
                printf("Dado %d adicionado na arvore\n", data);
            else
                printf("Dado %d nao foi adicionado!\n", data);
            TBinaryTree_dump(&tree);
            printf("\n");
        }

        printf("\n\n\n");
        TBinaryTree_show(&tree,printNode);
    }

    return 0;
}
