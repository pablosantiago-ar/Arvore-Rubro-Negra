# Arvore-Rubro-Negra para índice de arquivos

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![JavaScript](https://img.shields.io/badge/javascript-%23323330.svg?style=for-the-badge&logo=javascript&logoColor=%23F7DF1E)
![HTML5](https://img.shields.io/badge/html5-%23E34F26.svg?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/css3-%231572B6.svg?style=for-the-badge&logo=css3&logoColor=white)

## Sobre o projeto

Este projeto implementa uma Árvore Rubro-Negra (Red-Black Tree) para servir como um índice eficiente para arquivos. O principal objetivo é demonstrar como essa estrutura de dados auto-balanceável pode ser utilizada para otimizar operações de busca, inserção e remoção em grandes volumes de dados, simulando um sistema de indexação de arquivos. A Árvore Rubro-Negra garante um tempo logarítmico para essas operações, fundamental para manter a performance mesmo com muitos registros.

## pré requisitos
Certifique-se de ter os seguintes softwares instalados em sua máquina:

* **Compilador C (GCC/Clang)**: Essencial para compilar o código C.
    * No Linux/macOS, você geralmente já tem o GCC/Clang ou pode instalá-los via gerenciador de pacotes.
    * Para usuários de Windows, é recomendado instalar o **[MinGW](https://osdn.net/projects/mingw/releases/)** (Minimalist GNU for Windows), que inclui o GCC.
    * Você pode verificar a instalação abrindo o terminal/prompt de comando e digitando:
        ```bash
        gcc --version
        ```
* **Git**: Para clonar o repositório do GitHub.
    * Verifique a instalação com:
        ```bash
        git --version
        ```
    * Baixe em [git-scm.com](https://git-scm.com/).
* **Navegador Web**: Qualquer navegador moderno (Chrome, Firefox, Edge) para abrir a interface HTML.
* **Servidor Web Local (Opcional, mas Recomendado)**: Para servir os arquivos HTML, CSS e JavaScript.
    * **Com Node.js (se tiver npm):**
        ```bash
        npm install -g http-server
        ```

### 1. Clonando o Repositório

```bash
git clone https://github.com/pablosantiago-ar/Arvore-Rubro-Negra
```
## Instrução de instalação
* rode no terminal
 ```
   node server.js (Não feche o terminal)
 ```
* No navegador de sua preferência, cole o endereço que for fornecido no terminal
  
## Principais funcionalidades

Gerenciamento de Nós e Arquivos:

* Inserção de arquivos: Adiciona novos registros ao índice, criando um arquivo .txt correspondente para cada nó inserido e garantindo o balanceamento da árvore.

* Busca de arquivos: Permite encontrar registros no índice.

* Remoção de arquivos: Remove registros do índice, excluindo o nó da árvore e o arquivo .txt correspondente do sistema.

* Balanceamento Automático (Árvore Rubro-Negra):
Implementação de rotações à direita e à esquerda (rotacaoDir, rotacaoEsq).

* Funções de correção (RB_insert_fixup, RB_delete_fixup) que garantem as propriedades de auto-balanceamento da Árvore Rubro-Negra em todas as operações de inserção e remoção.


