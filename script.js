document.addEventListener('DOMContentLoaded', () => {
    //
    // PASSO 1: Cole aqui o JSON que você copiou da saída do seu programa C.
    //
    const treeData = {"value": 8,"color": "black","left": {"value": 5,"color": "black","left": {"value": 1,"color": "red","left": null,"right": null},"right": {"value": 6,"color": "red","left": null,"right": null}},"right": {"value": 12,"color": "red","left": {"value": 10,"color": "black","left": {"value": 9,"color": "red","left": null,"right": null},"right": null},"right": {"value": 72,"color": "black","left": {"value": 50,"color": "red","left": null,"right": null},"right": null}}};

    const treeContainer = document.getElementById('tree-container');
    
    // Limpa a área e cria a base para o desenho.
    treeContainer.innerHTML = ''; // Limpa tudo
    const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    svg.id = 'lines-container';
    treeContainer.appendChild(svg);
    
    const containerWidth = treeContainer.offsetWidth;

    // Se houver dados na árvore, inicia o processo de desenho.
    if (treeData) {
        drawTree(treeData, containerWidth / 2, 50, containerWidth / 4);
    }
});


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