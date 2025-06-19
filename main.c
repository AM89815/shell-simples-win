#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

#define LIMITE_STRINGS_ARGS 32
#define DELIMITADOR_ARGS " \t\r\n\a"
#define DELIMITADOR_CMD "&"

struct lista_comandos_lidos *processar_linha_lida(char *linha);
char **processar_linha(char *linha, char *delimitador_ARGS);
int verificar_shell_cmds(char **args);

/*
    **Comandos internos do shell**
    Para criar um novo comando interno é necessario:
    1- Adicionar uma string com o nome do comando em shell_cmds[] no final da lista;
    2- Criar uma nova função para o comando Ex: int shell_cmd_novocomando(char **args) 
       retornando 0;
    3- Adicionar a chamada da função em no swtich case da função verificar_shell_cmds
       obedecendo a ordem da lista de comandos shell_cmds[];
*/

char *shell_cmds[] = {
    "exit",   // sair
    "help",   // ajuda
    "cd",     // mudar de diretório
    "dir",    // listar arquivos e diretórios
    "cls",    // limpar a tela
    "pwd",    // mostrar o diretório atual
    "mkdir",  // criar diretório
    "rmdir",  // remover diretório vazio
    "del",    // remover arquivo
    "type",   // mostrar conteúdo de arquivo
    "copy",   // copiar arquivo
    "move"    // mover/renomear arquivo
};

int shell_cmds_tamanho();
int shell_cmd_exit(char **args);
int shell_cmd_help(char **args);
int shell_cmd_cd(char **args);
int shell_cmd_dir(char **args);
int shell_cmd_cls(char **args);
int shell_cmd_pwd(char **args);
int shell_cmd_mkdir(char **args);
int shell_cmd_rmdir(char **args);
int shell_cmd_del(char **args);
int shell_cmd_type(char **args);
int shell_cmd_copy(char **args);
int shell_cmd_move(char **args);

struct lista_comandos_lidos {
    struct comando *cmd;
    int tamanho;
};

struct comando {
    char **args;
    int index;
};

int main() {
    char buf[1024];
    int pid, status = 0;
    int executar = 1, i = 0;
    char **args;
    char prompt[MAX_PATH + 4];

    struct lista_comandos_lidos *lista_cmds_lidos = malloc(sizeof(struct lista_comandos_lidos));
    struct comando cmd;

    lista_cmds_lidos->tamanho = 0;

    // O comando exit retorna -1 encerrando o loop
    while (executar >= 0) {
        if (i >= lista_cmds_lidos->tamanho) {
            char cwd[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, cwd);
            snprintf(prompt, sizeof(prompt), "%s> ", cwd);
            printf("%s", prompt);
            fgets(buf, 1024, stdin);
            lista_cmds_lidos = processar_linha_lida(buf);
            i = 0;
        }

        cmd = (struct comando)lista_cmds_lidos->cmd[i];

        if (cmd.args[0] != NULL) {
            executar = verificar_shell_cmds(cmd.args);
        } else {
            executar = 0;
        }

        // Retorno de 1 do verificar_shell_cmds indica que não é um comando shell
        if (executar == 1) {
            pid = _spawnvp(_P_NOWAIT, cmd.args[0], (const char * const *)cmd.args);
            if (pid == -1) {
                printf("shell: erro ao executar o comando %s\n", cmd.args[0]);
            } else {
                if (cmd.index >= (lista_cmds_lidos->tamanho - 1)) {
                    _cwait(&status, pid, 0);
                }
            }
        }

        // Incrementa o contador para o proximo comando
        i++;
    }
    return 0;
}

struct lista_comandos_lidos *processar_linha_lida(char *linha) {
    int pos = 0, tamanho = 1;
    char **linha_cmd;
    struct lista_comandos_lidos *resultado = malloc(sizeof(struct lista_comandos_lidos));
    resultado->cmd = malloc(sizeof(struct comando));

    linha_cmd = processar_linha(linha, DELIMITADOR_CMD);

    while (linha_cmd[pos] != NULL) {
        resultado->cmd[pos].index = pos;
        resultado->cmd[pos].args = processar_linha(linha_cmd[pos], DELIMITADOR_ARGS);
        pos++;
        resultado->cmd = realloc(resultado->cmd, (pos + 1) * sizeof(struct comando));
        resultado->tamanho = pos;
    }

    return resultado;
}

char **processar_linha(char *linha, char *delimitador_ARGS) {
    int pos = 0;
    char *str;
    int tamanho_strs = LIMITE_STRINGS_ARGS;
    char **resultado = malloc(tamanho_strs * sizeof(char *));

    str = strtok(linha, delimitador_ARGS);
    while (str != NULL) {
        resultado[pos] = str;
        pos++;

        if (pos >= tamanho_strs) {
            tamanho_strs += LIMITE_STRINGS_ARGS;
            resultado = realloc(resultado, tamanho_strs * sizeof(char *));
        }

        str = strtok(NULL, delimitador_ARGS);
    }

    resultado[pos] = NULL;
    return resultado;
}

int verificar_shell_cmds(char **args) {
    int i;
    int index_shell_cmd = -1;
    for (i = 0; i < shell_cmds_tamanho(); i++) {
        if (strcasecmp(args[0], shell_cmds[i]) == 0) {
            index_shell_cmd = i;
        }
    }

    switch (index_shell_cmd) {
        case 0:  return shell_cmd_exit(args);
        case 1:  return shell_cmd_help(args);
        case 2:  return shell_cmd_cd(args);
        case 3:  return shell_cmd_dir(args);
        case 4:  return shell_cmd_cls(args);
        case 5:  return shell_cmd_pwd(args);
        case 6:  return shell_cmd_mkdir(args);
        case 7:  return shell_cmd_rmdir(args);
        case 8:  return shell_cmd_del(args);
        case 9:  return shell_cmd_type(args);
        case 10: return shell_cmd_copy(args);
        case 11: return shell_cmd_move(args);
        default: return 1;
    }
}

int shell_cmds_tamanho() {
    return sizeof(shell_cmds) / sizeof(char *);
}

int shell_cmd_exit(char **args) {
    return -1;
}

int shell_cmd_help(char **args) {
    int i;
    printf("==================================================\n");
    printf("*Shell simples para Windows - v1.1 (06/2025)\n");
    printf("*Desenvolvido por Aurelio Miguel\n");
    printf("==================================================\n");
    printf("*Lista de comandos internos: \n");

    for (i = 0; i < shell_cmds_tamanho(); i++) {
        printf("-%s\n", shell_cmds[i]);
    }
    printf("==================================================\n");

    return 0;
}

int shell_cmd_cd(char **args) {
    if (args[1] == NULL) {
        printf("shell: falta informar o diretorio para o comando cd...\n");
    } else {
        if (chdir(args[1]) != 0) {
            printf("shell: erro ao executar o comando cd %s...\n", args[1]);
        }
    }
    return 0;
}

int shell_cmd_dir(char **args) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char search_path[MAX_PATH] = ".\\*";
    if (args[1] != NULL) {
        snprintf(search_path, MAX_PATH, "%s\\*", args[1]);
    }
    hFind = FindFirstFile(search_path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("shell: erro ao listar o diretório\n");
        return 0;
    }
    do {
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            printf("%s\n", findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
    return 0;
}

int shell_cmd_cls(char **args) {
    system("cls");
    return 0;
}

int shell_cmd_pwd(char **args) {
    char cwd[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, cwd)) {
        printf("%s\n", cwd);
    } else {
        printf("shell: erro ao obter diretório atual\n");
    }
    return 0;
}

int shell_cmd_mkdir(char **args) {
    if (args[1] == NULL) {
        printf("shell: falta informar o nome do diretório para mkdir...\n");
    } else {
        if (CreateDirectory(args[1], NULL)) {
            printf("Diretório criado: %s\n", args[1]);
        } else {
            printf("shell: erro ao criar diretório %s...\n", args[1]);
        }
    }
    return 0;
}

int shell_cmd_rmdir(char **args) {
    if (args[1] == NULL) {
        printf("shell: falta informar o nome do diretório para rmdir...\n");
    } else {
        if (RemoveDirectory(args[1])) {
            printf("Diretório removido: %s\n", args[1]);
        } else {
            printf("shell: erro ao remover diretório %s...\n", args[1]);
        }
    }
    return 0;
}

int shell_cmd_del(char **args) {
    if (args[1] == NULL) {
        printf("shell: falta informar o nome do arquivo para del...\n");
    } else {
        if (DeleteFile(args[1])) {
            printf("Arquivo removido: %s\n", args[1]);
        } else {
            printf("shell: erro ao remover arquivo %s...\n", args[1]);
        }
    }
    return 0;
}

int shell_cmd_type(char **args) {
    if (args[1] == NULL) {
        printf("shell: falta informar o nome do arquivo para type...\n");
    } else {
        FILE *file = fopen(args[1], "r");
        if (file == NULL) {
            printf("shell: erro ao abrir arquivo %s...\n", args[1]);
        } else {
            char line[256];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        }
    }
    return 0;
}

int shell_cmd_copy(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("shell: uso: copy <origem> <destino>\n");
    } else {
        if (CopyFile(args[1], args[2], FALSE)) {
            printf("Arquivo copiado de %s para %s\n", args[1], args[2]);
        } else {
            printf("shell: erro ao copiar de %s para %s\n", args[1], args[2]);
        }
    }
    return 0;
}

int shell_cmd_move(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("shell: uso: move <origem> <destino>\n");
    } else {
        if (MoveFile(args[1], args[2])) {
            printf("Arquivo movido/renomeado de %s para %s\n", args[1], args[2]);
        } else {
            printf("shell: erro ao mover/renomear de %s para %s\n", args[1], args[2]);
        }
    }
    return 0;
}
