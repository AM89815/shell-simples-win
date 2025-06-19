# Shell simples para Windows

Um shell simples para Windows, desenvolvido em C, que implementa comandos internos comuns do CMD e permite executar programas externos.

## Funcionalidades

- **Prompt dinâmico:** Mostra sempre o diretório atual, como no CMD do Windows.
- **Comandos internos:**
  - `cd <diretório>` — Muda o diretório atual.
  - `dir [diretório]` — Lista arquivos e pastas do diretório (padrão: atual).
  - `cls` — Limpa a tela.
  - `pwd` — Mostra o diretório atual.
  - `mkdir <diretório>` — Cria um novo diretório.
  - `rmdir <diretório>` — Remove um diretório vazio.
  - `del <arquivo>` — Remove um arquivo.
  - `type <arquivo>` — Mostra o conteúdo de um arquivo de texto.
  - `copy <origem> <destino>` — Copia arquivos.
  - `move <origem> <destino>` — Move ou renomeia arquivos.
  - `help` — Lista os comandos internos disponíveis.
  - `exit` — Encerra o shell.
- **Execução de programas externos:** Qualquer comando não reconhecido como interno é executado como programa externo, se disponível no PATH.

## Como compilar

Requisitos:
- [MinGW-w64](https://www.mingw-w64.org/) instalado e configurado no PATH.
- Windows.

No terminal, execute:
```sh
gcc main.c -o shell.exe
```

## Como usar

No terminal, execute:
```sh
shell.exe
```

Você verá um prompt semelhante ao CMD, mostrando o diretório atual. Digite comandos internos ou externos normalmente.

Exemplos:
```sh
C:\Users\aurel\dev\c\shell> dir
C:\Users\aurel\dev\c\shell> cd ..
C:\Users\aurel\dev\c> cls
C:\Users\aurel\dev\c> pwd
C:\Users\aurel\dev\c
C:\Users\aurel\dev\c> mkdir teste
C:\Users\aurel\dev\c> copy arquivo.txt copia.txt
C:\Users\aurel\dev\c> del copia.txt
C:\Users\aurel\dev\c> help
```

## Estrutura do código

- **main.c**: Implementa toda a lógica do shell, comandos internos e execução de comandos externos.
- **Funções principais:**
  - `main()`: Loop principal do shell.
  - `verificar_shell_cmds()`: Verifica se o comando é interno.
  - `shell_cmd_*()`: Implementações dos comandos internos.
  - `processar_linha_lida()`, `processar_linha()`: Processamento da entrada do usuário.

## Observações

- O shell foi desenvolvido para Windows e utiliza funções específicas do sistema, como `GetCurrentDirectory`, `FindFirstFile`, `CopyFile`, `MoveFile` e `_spawnvp`.
- Não suporta comandos POSIX/Linux como `ls`, `clear`, etc.
- Para adicionar novos comandos internos, siga as instruções no início do arquivo `main.c`.

---

Desenvolvido por Aurélio Miguel  
Junho 2025