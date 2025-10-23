# jogoAED 🎮

Projeto base em **C + Raylib** com **submódulo Git** para colaborar no GitHub.

## ✅ Requisitos
- Git + Make
- Compilador C (GCC/Clang)
- **NÃO** precisa instalar a Raylib no sistema — ela vem como submódulo.

### Windows (MSYS2 recomendado)
1. Instale: https://www.msys2.org/
2. Abra **MSYS2 MinGW64** e rode:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc make git
   ```

### Linux (Debian/Ubuntu)
```bash
sudo apt update && sudo apt install build-essential git make
```

### macOS (Homebrew)
```bash
xcode-select --install
brew install git make
```

## 🚀 Clonar e compilar
```bash
git clone --recurse-submodules https://github.com/SEU_USUARIO/jogoAED.git
cd jogoAED
make setup    # inicializa a raylib (primeira vez)
make          # compila o jogo e a raylib
make run      # executa
```

> Se você já clonou sem `--recurse-submodules`, rode `make setup` ou:
> ```bash
> git submodule update --init --recursive
> ```

## 🧱 Estrutura
```
jogoAED/
├─ src/                # código-fonte do jogo
│  └─ main.c
├─ include/            # headers do projeto
├─ assets/             # mídias (imagens, sons, fontes)
├─ external/
│  └─ raylib/          # submódulo git (código-fonte da raylib)
├─ .vscode/            # tarefas de build para VS Code
├─ Makefile
└─ README.md
```

## 🔧 Comandos úteis
- `make` – compila o projeto
- `make run` – executa o binário
- `make clean` – apaga objetos (`build/`)
- `make distclean` – também limpa `bin/` e artefatos da raylib
- `make setup` – inicializa/atualiza o submódulo

## 👥 Fluxo de colaboração (GitHub)
1. Crie uma branch: `git checkout -b feature/nome`
2. Faça commits pequenos e descritivos
3. Abra um Pull Request para `main`
4. Use **Issues** e **Projects** para organizar as tarefas

## 🧪 Teste rápido
Setas movimentam um círculo na tela. `Esc` fecha o jogo.
