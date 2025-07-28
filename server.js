const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');
const { promisify } = require('util');

const execPromise = promisify(exec);

const app = express();
const PORT = 3000;

// Estado da árvore (inicia com valores e conteúdos)
let treeData = [
    { value: 8, content: "Conteúdo do nó 8" },
    { value: 12, content: "Conteúdo do nó 12" },
    { value: 5, content: "Conteúdo do nó 5" },
    { value: 10, content: "Conteúdo do nó 10" },
    { value: 1, content: "Conteúdo do nó 1" },
    { value: 6, content: "Conteúdo do nó 6" },
    { value: 72, content: "Conteúdo do nó 72" },
    { value: 9, content: "Conteúdo do nó 9" },
    { value: 50, content: "Conteúdo do nó 50" }
];

// Caminhos dos arquivos
const publicDir = path.join(__dirname, 'public');
const cFile = path.join(__dirname, 'arvore.c');
const exeFile = path.join(__dirname, process.platform === 'win32' ? 'arvore.exe' : 'arvore');
const publicJson = path.join(publicDir, 'tree.json');

app.use(express.static(publicDir));
app.use(express.json());

/**
 * Compila e executa o programa C para gerar a árvore e os arquivos.
 */
async function generateTree() {
    try {
        await execPromise(`gcc "${cFile}" -std=c99 -o "${exeFile}" -lm`);
        console.log('Compilação bem-sucedida.');
    } catch (err) {
        console.error('Erro de compilação:', err.stderr);
        throw new Error('Erro ao compilar o código C.');
    }

    // Monta os argumentos para o programa C, escapando o conteúdo para a linha de comando.
    const args = treeData.map(d => `${d.value} "${d.content.replace(/"/g, '\\"')}"`).join(' ');
    const command = `"${exeFile}" ${args}`;

    try {
        const { stdout } = await execPromise(command);
        console.log('Execução do programa C:', stdout);
    } catch (err) {
        console.error('Erro na execução do programa C:', err.stderr);
        throw new Error('Erro ao executar o programa C para gerar a árvore.');
    }

    // Move o tree.json gerado para a pasta /public
    const generatedJsonPath = path.join(__dirname, 'tree.json');
    if (fs.existsSync(generatedJsonPath)) {
        fs.renameSync(generatedJsonPath, publicJson);
        console.log('tree.json movido para a pasta public.');
    } else {
        fs.writeFileSync(publicJson, 'null');
        console.log('tree.json não foi gerado, criando arquivo nulo para árvore vazia.');
    }
}

app.get('/', (req, res) => {
    res.sendFile(path.join(publicDir, 'index.html'));
});

// ROTA PARA LER O CONTEÚDO DE UM ARQUIVO
app.get('/file/:value', (req, res) => {
    const value = req.params.value;
    const filePath = path.join(__dirname, `node_${value}.txt`);

    fs.readFile(filePath, 'utf8', (err, data) => {
        if (err) {
            return res.status(404).json({ success: false, message: 'Arquivo do nó não encontrado.' });
        }
        res.status(200).json({ success: true, content: data });
    });
});

// ROTA DE INSERÇÃO ATUALIZADA
app.post('/insert', async (req, res) => {
    const { value, content } = req.body;
    const numValue = parseInt(value, 10);

    if (isNaN(numValue)) {
        return res.status(400).json({ success: false, message: 'Valor fornecido é inválido.' });
    }
    if (treeData.some(d => d.value === numValue)) {
        return res.status(409).json({ success: false, message: 'O valor já existe na árvore.' });
    }

    treeData.push({ value: numValue, content: content || "" });
    try {
        await generateTree();
        res.status(200).json({ success: true, message: `Valor ${numValue} inserido com sucesso!` });
    } catch (error) {
        treeData = treeData.filter(d => d.value !== numValue);
        res.status(500).json({ success: false, message: error.message });
    }
});

// ROTA DE REMOÇÃO ATUALIZADA
app.post('/delete', async (req, res) => {
    const value = parseInt(req.body.value, 10);
    if (isNaN(value)) {
        return res.status(400).json({ success: false, message: 'Valor fornecido é inválido.' });
    }
    if (!treeData.some(d => d.value === value)) {
        return res.status(404).json({ success: false, message: 'O valor não foi encontrado.' });
    }

    const originalData = [...treeData];
    treeData = treeData.filter(d => d.value !== value);

    // Deleta o arquivo físico do nó
    const filePath = path.join(__dirname, `node_${value}.txt`);
    if(fs.existsSync(filePath)) {
        fs.unlinkSync(filePath);
    }
    
    try {
        await generateTree();
        res.status(200).json({ success: true, message: `Valor ${value} removido com sucesso!` });
    } catch (error) {
        treeData = originalData;
        res.status(500).json({ success: false, message: error.message });
    }
});

app.get('/search', (req, res) => {
    const value = parseInt(req.query.value, 10);
    if (isNaN(value)) {
        return res.status(400).json({ found: false, message: 'Valor de busca inválido.' });
    }
    const found = treeData.some(d => d.value === value);
    if (found) {
        res.status(200).json({ found: true, message: `O valor ${value} foi encontrado na árvore!` });
    } else {
        res.status(404).json({ found: false, message: `O valor ${value} não foi encontrado.` });
    }
});

app.listen(PORT, () => {
    console.log(`Servidor rodando em http://localhost:${PORT}`);
    generateTree().catch(err => {
        console.error("Erro ao gerar a árvore inicial:", err);
    });
});