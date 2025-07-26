const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 3000;

// Servir arquivos estáticos da pasta public
app.use(express.static(path.join(__dirname, 'public')));

// Rota principal - página com botão para gerar árvore
app.get('/', (req, res) => {
  res.send(`
    <h1>Gerador de Árvore</h1>
    <button onclick="gerarArvore()">Gerar Árvore</button>
    <p id="status"></p>
    <script>
      function gerarArvore() {
        document.getElementById('status').innerText = 'Gerando árvore...';
        fetch('/generate')
          .then(resp => resp.text())
          .then(msg => {
            document.getElementById('status').innerText = msg;
            // Você pode aqui carregar o JSON e mostrar a árvore no front, se quiser
          })
          .catch(err => {
            document.getElementById('status').innerText = 'Erro ao gerar árvore.';
            console.error(err);
          });
      }
    </script>
  `);
});

// Rota para gerar a árvore
app.get('/generate', (req, res) => {
  const cFile = path.join(__dirname, 'arvore.c');
  const exeFile = path.join(__dirname, 'arvore.exe');
  const jsonFile = path.join(__dirname, 'tree.json');
  const publicJson = path.join(__dirname, 'public', 'tree.json');

  // Garante que a pasta public existe
  if (!fs.existsSync(path.join(__dirname, 'public'))) {
    fs.mkdirSync(path.join(__dirname, 'public'), { recursive: true });
  }

  // Compila o arquivo C para exe
  exec(`gcc "${cFile}" -o "${exeFile}"`, (compileErr, stdout, stderr) => {
    if (compileErr) {
      console.error('Erro ao compilar:', compileErr);
      return res.status(500).send('Erro ao compilar arquivo C');
    }

    console.log('Compilação feita com sucesso!');

    // Executa o exe gerado
    exec(`"${exeFile}"`, (execErr, stdout, stderr) => {
      if (execErr) {
        console.error('Erro ao executar o exe:', execErr);
        return res.status(500).send('Erro ao executar programa');
      }

      console.log('Programa executado com sucesso!');

      // Verifica se tree.json foi criado
      if (!fs.existsSync(jsonFile)) {
        console.error('Arquivo tree.json não encontrado após execução.');
        return res.status(500).send('JSON da árvore não encontrado');
      }

      // Move tree.json para a pasta public
      fs.rename(jsonFile, publicJson, (renameErr) => {
        if (renameErr) {
          console.error('Erro ao mover tree.json para public:', renameErr);
          return res.status(500).send('Erro ao mover arquivo JSON');
        }

        console.log('Arquivo tree.json movido para public com sucesso!');
        return res.send('Árvore gerada com sucesso!');
      });
    });
  });
});

app.listen(PORT, () => {
  console.log(`Servidor rodando em http://localhost:${PORT}`);
});
