document.addEventListener('DOMContentLoaded', () => {
    loadAndDrawTree();
});

async function loadAndDrawTree() {
    try {
        const response = await fetch('/tree.json?t=' + new Date().getTime()); // evita cache
        if (!response.ok) throw new Error(`Erro HTTP: ${response.status}`);

        const treeData = await response.json();
        console.log("✅ Dados carregados:", treeData);

        const treeContainer = document.getElementById('tree-container');
        treeContainer.innerHTML = ''; // limpa antes de desenhar

        const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
        svg.id = 'lines-container';
        treeContainer.appendChild(svg);

        const containerWidth = treeContainer.offsetWidth;
        if (treeData) {
            drawTree(treeData, containerWidth / 2, 50, containerWidth / 4);
        }
    } catch (error) {
        console.error("❌ Erro ao carregar a árvore:", error);
        document.getElementById('tree-container').innerHTML =
            `<p class="error-msg">Erro ao carregar <strong>tree.json</strong>. Verifique o servidor.</p>`;
    }
}

function drawTree(node, x, y, hSpacing) {
    if (!node) return;

    const container = document.getElementById('tree-container');
    const childY = y + 90;

    // Desenha as conexões primeiro
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

    // Desenha o nó
    const nodeElement = document.createElement('div');
    nodeElement.className = `node ${node.color}`;
    nodeElement.innerText = node.value;

    nodeElement.style.left = `${x - 25}px`;
    nodeElement.style.top = `${y - 25}px`;

    container.appendChild(nodeElement);
}

function drawLine(x1, y1, x2, y2) {
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
