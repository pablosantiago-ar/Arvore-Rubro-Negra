#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BLACK 0
#define RED 1


typedef struct tagNode {
    int size;
    int color;
    int level;
    char *data;
    char *filename;
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


bool TNode_create(TNode **node,char *data, int size,int level, TNode *parent, int cor, const char *filename);
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

bool TBinaryTree_create(TBinaryTree *tree,char *data,int dataSize,int (*comparacao)(char *data1, char *data2), const char *filename);
void TBinaryTree_destroy(TBinaryTree *tree);
bool TBinaryTree_add(TBinaryTree *tree,char *data, const char *filename);
bool TBinaryTree_addNode(TBinaryTree *tree,TNode *node1, TNode *node2,int level);
void TBinaryTree_dump(TBinaryTree *tree);
void TBinaryTree_show(TBinaryTree *tree, void (*print)(char *data));
bool TBinaryTree_search(TBinaryTree *tree, char *data);
bool TBinaryTree_delete(TBinaryTree *tree, char *data);
void TNode_updateLevels(TNode* node, int level);

void TNode_updateLevels(TNode* node, int level) {
    if (node == NULL) {
        return;
    }
    node->level = level;
    TNode_updateLevels(node->left, level + 1);
    TNode_updateLevels(node->right, level + 1);
}

bool TNode_create(TNode **node,char *data, int size,int level,TNode *parent, int cor, const char *filename){
    if(((*node) = malloc(sizeof(TNode))) != NULL) {
        if(((*node)->data = malloc(size)) != NULL) {
            memcpy((*node)->data,data,size);
            (*node)->size = size;
            (*node)->parent = parent;
            (*node)->level  = level;
            (*node)->left = NULL;
            (*node)->right = NULL;
            (*node)->color = cor;

            (*node)->filename = strdup(filename);
            FILE *fp = fopen((*node)->filename, "w");
            if (fp != NULL) {
                fclose(fp);
            } else {
                fprintf(stderr, "Erro ao criar arquivo para o nó: %s\n", (*node)->filename);
                free((*node)->filename);
                free((*node)->data);
                free(*node);
                return false;
            }

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

    if (node->filename != NULL){
        if(remove(node->filename) != 0){
            fprintf(stderr, "Erro ao excluir arquivo %s\n", node->filename);
        }
        free(node->filename);
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
        printf("(%d)", node->color);
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

    if (node_aux->parent == NULL) {
    } else if (node_aux == node_aux->parent->right) {
        node_aux->parent->right = filho_esq;
    } else {
        node_aux->parent->left = filho_esq;
    }

    filho_esq->right = node_aux;
    node_aux->parent = filho_esq;

    *node = filho_esq;

    TNode_updateLevels(*node, (*node)->level);
}

void rotacaoEsq(TNode **node){
    if (node == NULL || *node == NULL || (*node)->right == NULL) {
            return;
    }

    TNode *node_aux = *node;
    TNode *filho_dir = node_aux->right;

    node_aux->right = filho_dir->left;
    if (filho_dir->left != NULL) {
        filho_dir->left->parent = node_aux;
    }

    filho_dir->parent = node_aux->parent;

    if (node_aux->parent == NULL) {
    } else if (node_aux == node_aux->parent->left) {
        node_aux->parent->left = filho_dir;
    } else {
        node_aux->parent->right = filho_dir;
    }

    filho_dir->left = node_aux;
    node_aux->parent = filho_dir;

    *node = filho_dir;

    TNode_updateLevels(*node, (*node)->level);
}

bool TBinaryTree_create(TBinaryTree *tree,char *data,int dataSize,int (*comparacao)(char *data1, char *data2), const char *filename) {
    if(data != NULL && dataSize > 0 && filename != NULL) {
        if(TNode_create(&(tree->root),data,dataSize,0,NULL, 0, filename)) {
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

void RB_insert_fixup(TBinaryTree *tree, TNode *z) {
    TNode *tio;

    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            tio = z->parent->parent->right;
            if (tio != NULL && tio->color == RED) {
                z->parent->color = BLACK;
                tio->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotacaoEsq(&z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotacaoDir(&(z->parent->parent));
            }
        } else {
            tio = z->parent->parent->left;
            if (tio != NULL && tio->color == RED) {
                z->parent->color = BLACK;
                tio->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotacaoDir(&z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotacaoEsq(&(z->parent->parent));
            }
        }
        if (z == tree->root) {
            break;
        }
    }
    tree->root->color = BLACK;
}

bool TBinaryTree_add(TBinaryTree *tree,char *data, const char *filename){
    TNode *node;
    if(data != NULL && filename != NULL){
        if(!TNode_create(&node,data,tree->dataSize,0,NULL, 1, filename)) {
            return false;

        if (tree->root == NULL) {
            tree->root = node;
            node->color = BLACK;
            tree->numberNodes = 1;
            tree->deepth = 0;
            return true;
        }

        TNode *current = tree->root;
        TNode *parent = NULL;
        int cmp_result;

        while (current != NULL) {
            parent = current;
            cmp_result = tree->comparacao(data, current->data);
            if (cmp_result == 0) { // Dado já existe
                TNode_destroy(node); // Destrói o nó e o arquivo recém-criados
                return false;
            } else if (cmp_result > 0) {
                current = current->right;
            } else {
                current = current->left;
            }
        }

        node->parent = parent;
        node->level = parent->level + 1;
        if (cmp_result > 0) {
            parent->right = node;
        } else {
            parent->left = node;
        }

        tree->numberNodes++;
        if(node->level > tree->deepth){
            tree->deepth = node->level;
        }

        RB_insert_fixup(tree, node);
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


void RB_transplant(TBinaryTree *tree, TNode *u, TNode *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

TNode* tree_minimum(TNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

void RB_delete_fixup(TBinaryTree *tree, TNode *x) {
    TNode *w;

    while (x != NULL && x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotacaoEsq(&(x->parent));
                w = x->parent->right;
            }
            if (w != NULL && (w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else if (w != NULL) {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    rotacaoDir(&(w));
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                rotacaoEsq(&(x->parent));
                x = tree->root;
            } else {
                 x = tree->root;
            }
        } else {
            w = x->parent->left;
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotacaoDir(&(x->parent));
                w = x->parent->left;
            }
            if (w != NULL && (w->right == NULL || w->right->color == BLACK) && (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else if (w != NULL) {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    rotacaoEsq(&(w));
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                rotacaoDir(&(x->parent));
                x = tree->root;
            } else {
                 x = tree->root;
            }
        }
    }
    if (x != NULL) {
        x->color = BLACK;
    }
}

bool TBinaryTree_delete(TBinaryTree *tree, char *data) {
    TNode *z = tree->root;
    while(z != NULL) {
        int cmp = tree->comparacao(data, z->data);
        if (cmp == 0){
            break;
        }
        z = (cmp > 0) ? z->right : z->left;
    }

    if (z == NULL) {
        return false;
    }

    TNode *y = z;
    TNode *x;
    int y_original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        RB_transplant(tree, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        RB_transplant(tree, z, z->left);
    } else {
        y = tree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != NULL) {
                x->parent = y;
            }
        } else {
            RB_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        RB_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;

        y->level = z->level;
        TNode_updateLevels(y, y->level);
    }

    tree->numberNodes--;

    if (y_original_color == BLACK) {
        RB_delete_fixup(tree, x);
    }

    if(z->filename != NULL){
        if(remove(z->filename) != 0){
            fprintf(stderr, "Erro ao excluir arquivo %s\n", z->filename);
        }
        free(z->filename);
    }
    free(z->data);
    free(z);

    return true;
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

void TNode_export_json(FILE *file, TNode* node) {
    if (node == NULL) {
        fprintf(file, "null");
        return;
    }

    fprintf(file, "{");
    fprintf(file, "\"value\": %d,", *((int*)node->data));
    fprintf(file, "\"color\": \"%s\",", (node->color == RED ? "red" : "black"));

    fprintf(file, "\"left\": ");
    TNode_export_json(file, node->left);

    fprintf(file, ",\"right\": ");
    TNode_export_json(file, node->right);

    fprintf(file, "}");
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

int main(int argc, char *argv[]) {
    TBinaryTree tree;
    bool tree_created = false;
    char filename_buffer[256];

    if (argc < 2) {
        FILE *json_file = fopen("tree.json", "w");
        if (json_file != NULL) {
            fprintf(json_file, "null");
            fclose(json_file);
        }
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        int value = atoi(argv[i]);
        snprintf(filename_buffer, sizeof(filename_buffer), "node_%d.txt", value);

        if (!tree_created) {
            if (TBinaryTree_create(&tree, (char*)&value, sizeof(value), compara_int, filename_buffer)) {
                tree_created = true;
            } else {
                fprintf(stderr, "Erro ao criar a árvore com a raiz %d.\n", value);
                return 1;
            }
        } else {
            if(!TBinaryTree_add(&tree, (char*)&value, filename_buffer)){
            }
        }
    }

    if (tree_created) {
        FILE *json_file = fopen("tree.json", "w");
        if (json_file == NULL) {
            perror("Erro ao abrir o arquivo tree.json para escrita");
            TBinaryTree_destroy(&tree);
            return 1;
        }
        TNode_export_json(json_file, tree.root);
        fclose(json_file);
        printf("tree.json gerado/atualizado com sucesso.\n");
        TBinaryTree_destroy(&tree);
    }

    return 0;
}