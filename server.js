const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');
const { promisify } = require('util');

const execPromise = promisify(exec);

const app = express();
const PORT = 3000;

let treeValues = [8, 12, 5, 10, 1, 6, 72, 9, 50];

const publicDir = path.join(__dirname, 'public');
const cFile = path.join(__dirname, 'arvore.c');
const exeFile = path.join(__dirname, process.platform === 'win32' ? 'arvore.exe' : 'arvore');
const publicJson = path.join(publicDir, 'tree.json');

app.use(express.static(publicDir));
app.use(express.json());

async function generateTree() {
    try {
        await execPromise(`gcc "${cFile}" -o "${exeFile}" -lm`);
        console.log('Compilação bem-sucedida.');
    } catch (err) {
        console.error('Erro de compilação:', err.stderr);
        throw new Error('Erro ao compilar o código C.');
    }

    const files = fs.readdirSync(__dirname);
    files.forEach(file => {
        if (file.startsWith('node_') && file.endsWith('.txt')) {
            try {
                fs.unlinkSync(path.join(__dirname, file));
            } catch (err) {
                console.warn(`Não foi possível remover o arquivo antigo: ${file}`);
            }
        }
    });

    const command = `"${exeFile}" ${treeValues.join(' ')}`;
    try {
        const { stdout } = await execPromise(command);
        console.log('Execução do programa C:', stdout);
    } catch (err) {
        console.error('Erro na execução do programa C:', err.stderr);
        throw new Error('Erro ao executar o programa C para gerar a árvore.');
    }

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

app.get('/generate', async (req, res) => {
    try {
        await generateTree();
        res.status(200).json({ success: true, message: 'Árvore gerada com sucesso!' });
    } catch (error) {
        res.status(500).json({ success: false, message: error.message });
    }
});

app.post('/insert', async (req, res) => {
    const value = parseInt(req.body.value, 10);
    if (isNaN(value)) {
        return res.status(400).json({ success: false, message: 'Valor fornecido é inválido.' });
    }
    if (treeValues.includes(value)) {
        return res.status(409).json({ success: false, message: 'O valor já existe na árvore.' });
    }

    treeValues.push(value);
    try {
        await generateTree();
        res.status(200).json({ success: true, message: `Valor ${value} inserido com sucesso!` });
    } catch (error) {
        treeValues = treeValues.filter(v => v !== value);
        res.status(500).json({ success: false, message: error.message });
    }
});

app.post('/delete', async (req, res) => {
    const value = parseInt(req.body.value, 10);
    if (isNaN(value)) {
        return res.status(400).json({ success: false, message: 'Valor fornecido é inválido.' });
    }
    if (!treeValues.includes(value)) {
        return res.status(404).json({ success: false, message: 'O valor não foi encontrado na árvore para remoção.' });
    }

    const originalValues = [...treeValues];
    treeValues = treeValues.filter(v => v !== value);
    try {
        await generateTree();
        res.status(200).json({ success: true, message: `Valor ${value} removido com sucesso!` });
    } catch (error) {
        treeValues = originalValues;
        res.status(500).json({ success: false, message: error.message });
    }
});

app.get('/search', (req, res) => {
    const value = parseInt(req.query.value, 10);
     if (isNaN(value)) {
        return res.status(400).json({ found: false, message: 'Valor de busca inválido.' });
    }
    const found = treeValues.includes(value);
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