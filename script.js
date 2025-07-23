document.addEventListener('DOMContentLoaded', () => {
    // A função principal agora carrega os dados antes de desenhar.
    loadAndDrawTree();
});

/**
 * Carrega os dados do arquivo tree.json e inicia o desenho.
 */
async function loadAndDrawTree() {
    try {
        // Faz uma requisição para carregar o arquivo local tree.json
        const response = await fetch('tree.json?t=' + new Date().getTime()); // O parâmetro extra evita cache
        
        // Verifica se a requisição foi bem-sucedida
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        // Converte a resposta em um objeto JSON
        const treeData = await response.json();

        // Pega os contêineres e inicia o desenho com os dados carregados
        const treeContainer = document.getElementById('tree-container');
        treeContainer.innerHTML = ''; // Limpa a área
        const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
        svg.id = 'lines-container';
        treeContainer.appendChild(svg);
        
        const containerWidth = treeContainer.offsetWidth;
        if (treeData) {
            drawTree(treeData, containerWidth / 2, 50, containerWidth / 4);
        }
    } catch (error) {
        console.error("Erro ao carregar ou desenhar a árvore:", error);
        // Exibe uma mensagem de erro na tela para o usuário
        const treeContainer = document.getElementById('tree-container');
        treeContainer.innerHTML = `<p class="error-msg">Falha ao carregar o arquivo <strong>tree.json</strong>. Verifique se o programa em C foi executado e se o servidor local está rodando.</p>`;
    }
}


function drawTree(node, x, y, hSpacing) {
    if (node === null) {
        return;
    }

    const container = document.getElementById('tree-container');
    const childY = y + 90;

    // --- 1. DESENHA FILHOS E LINHAS ---
    if (node.left) {
        const childX = x - hSpacing;
        drawLine(x, y, childX, childY);
        drawTree(node.left, childX, childY, hSpacing / 2);
    }
    
    if (node.right) {
        const childX = x + hSpacing;
        drawLine(x, y, childX, childY);
        drawTree(node.right, childX, childY, hSpacing / 2);
    }
    
    // --- 2. DESENHA O NÓ ATUAL ---
    const nodeElement = document.createElement('div');
    nodeElement.className = `node ${node.color}`; 
    nodeElement.innerText = node.value;

    nodeElement.style.left = `${x - 25}px`;
    nodeElement.style.top = `${y - 25}px`;
    
    container.appendChild(nodeElement);
}


function drawLine(x1, y1, x2, y2) {
    // Agora o 'svg' é pego de forma segura, pois já foi criado.
    const svg = document.getElementById('lines-container');
    const line = document.createElementNS('http://www.w3.org/2000/svg', 'line');
    
    line.setAttribute('x1', x1);
    line.setAttribute('y1', y1);
    line.setAttribute('x2', x2);
    line.setAttribute('y2', y2);
    line.setAttribute('stroke', '#555'); 
    line.setAttribute('stroke-width', 2);
    
    svg.appendChild(line);
}