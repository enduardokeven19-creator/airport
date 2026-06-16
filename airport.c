#include "airport.h"
#include "cJSON.h"

void cadastrar_voo(){
    v cad_voo;
    char linha[MAX_READ];
    int contador_id = -1;
    int x = 1;
    
    do {
        printf("\n----------Cadastrar voo----------\n");

        printf("\nDigite o nome do voo: ");
        scanf(" %[^\n]", cad_voo.nome_voo);

        printf("\nDigite a capacidade disponível: ");
        scanf("%u", &cad_voo.capacidade);

        printf("\nDigite o destino da viagem: ");
        scanf(" %[^\n]", cad_voo.destino);

        printf("\nDigite o horário: ");
        scanf(" %[^\n]", cad_voo.horario);

        printf("\nDigite o valor do voo: ");
        scanf("%f", &cad_voo.preco);

        printf("\nDigite o valor da primeira classe: ");
        scanf("%f", &cad_voo.preco_classe);

        if(contador_id == -1){
            contador_id = 0;
            FILE *verificar = fopen("voos.json", "r");
            if (verificar != NULL) {
                while (fgets(linha, sizeof(linha), verificar) != NULL) {
                    cJSON *teste_json = cJSON_Parse(linha);
                    if (teste_json != NULL) {
                        contador_id++;
                        cJSON_Delete(teste_json);
                    }
                }
                fclose(verificar);
            }
            cad_voo.id = contador_id + 1;
        } else {
            cad_voo.id = contador_id + 1;
        }
      
        cJSON *voo_disp = cJSON_CreateObject();
        cJSON_AddNumberToObject(voo_disp, "id", cad_voo.id);
        cJSON_AddStringToObject(voo_disp, "nome_voo", cad_voo.nome_voo);
        cJSON_AddStringToObject(voo_disp, "destino", cad_voo.destino);
        cJSON_AddStringToObject(voo_disp, "horário", cad_voo.horario);
        cJSON_AddNumberToObject(voo_disp, "capacidade", cad_voo.capacidade);
        cJSON_AddNumberToObject(voo_disp, "cap_primeira", cad_voo.capacidade / 2);
        cJSON_AddNumberToObject(voo_disp, "cap_economica", cad_voo.capacidade - (cad_voo.capacidade / 2));
        cJSON_AddNumberToObject(voo_disp, "preco", cad_voo.preco); 
        cJSON_AddNumberToObject(voo_disp, "preco_primeira_classe", cad_voo.preco_classe);

        char *texto_JSON = cJSON_PrintUnformatted(voo_disp);
        FILE *arquivo = fopen("voos.json", "a");

        if (arquivo != NULL) {
            fprintf(arquivo, "%s\n", texto_JSON);
            fflush(arquivo);
            fclose(arquivo);
            printf("\nVoo cadastrado com sucesso!");
            contador_id = cad_voo.id;
        } else {
            printf("\nErro ao cadastrar voo");
            cJSON_Delete(voo_disp);
            free(texto_JSON);
            return; 
        }
        
        free(texto_JSON);
        cJSON_Delete(voo_disp);

        printf("\nDeseja continuar? [1 para Sim / 0 para Não]: ");
        scanf("%d", &x);

    } while(x != 0);
}

void historico_voo(){
    char linha[MAX_READ];
    FILE *arquivo = fopen("voos.json", "r");
    
    if (arquivo != NULL) {
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            cJSON *voo_json = cJSON_Parse(linha);
            if(voo_json != NULL){
                int id = cJSON_GetObjectItem(voo_json, "id")->valueint;
                char *nome = cJSON_GetObjectItem(voo_json, "nome_voo")->valuestring;
                int capacidade = cJSON_GetObjectItem(voo_json, "capacidade")->valueint;
                int cap_primeira = cJSON_GetObjectItem(voo_json, "cap_primeira")->valueint;
                int cap_economica = cJSON_GetObjectItem(voo_json, "cap_economica")->valueint;
                char *destino = cJSON_GetObjectItem(voo_json, "destino")->valuestring;
                char *horario = cJSON_GetObjectItem(voo_json, "horário")->valuestring;
                float preco = (float)cJSON_GetObjectItem(voo_json, "preco")->valuedouble;
                float preco_classe = (float)cJSON_GetObjectItem(voo_json, "preco_primeira_classe")->valuedouble;

                printf("\nID %d | Voo: %s | Horário: %s | Destino %s | capacidade: %d | 1º Classe: %d | C.Econômica: %d | Preço-Classe econômica: %.2f | Preço-Primeira Classe %.2f\n", 
                       id, nome, horario, destino, capacidade, cap_primeira, cap_economica, preco, preco_classe);
                cJSON_Delete(voo_json);
            }
        }
        fclose(arquivo);
    } else {
        printf("\nArquivo não encontrado ou nenhum dado cadastrado\n");
        return;
    }
    
    #ifdef _WIN32
    system("pause");
    #else
    printf("\nPressione ENTER para voltar...");
    getchar(); getchar();
    #endif
}

void cadastrar_passageiro(){
    int opc = 1;

    do {
        p cad_passa; 
        char linha[MAX_READ];
        int escolher_id;
        int voo_encontrado = 0;
        int contador_id = 0;
        int assento_ocupado = 0;

        printf("\n---------Cadastrar passageiro----------\n");

        printf("Digite o nome do passageiro: ");
        scanf(" %[^\n]", cad_passa.nome_passageiro);

        printf("Digite o cpf do passageiro: ");
        scanf(" %[^\n]", cad_passa.cpf);

        printf("Digite a classe:\n[1] para primeira classe\n[0] para classe econômica\nOpção: ");
        scanf("%d", &cad_passa.classe); 

        printf("Digite o número do assento desejado (ex: 45): ");
        scanf("%u", &cad_passa.assento); 

        printf("\nDigite o ID do voo que deseja cadastrar: ");
        scanf("%d", &escolher_id);

        FILE *verificar_pass = fopen("passageiros.json", "r");
        if (verificar_pass != NULL) {
            while (fgets(linha, sizeof(linha), verificar_pass) != NULL) {
                cJSON *pass_salvo = cJSON_Parse(linha);
                if (pass_salvo != NULL) {
                    contador_id++; 

                    int voo_salvo = cJSON_GetObjectItem(pass_salvo, "id_voo")->valueint;
                    unsigned int assento_salvo = (unsigned int)cJSON_GetObjectItem(pass_salvo, "assento")->valueint;
                    int classe_salva = cJSON_GetObjectItem(pass_salvo, "classe")->valueint;

                    if (voo_salvo == escolher_id && assento_salvo == cad_passa.assento && classe_salva == cad_passa.classe) {
                        printf("\nErro: O assento %u já está ocupado nesta classe para este vôo!", cad_passa.assento);
                        assento_ocupado = 1;
                        cJSON_Delete(pass_salvo);
                        break;
                    }
                    cJSON_Delete(pass_salvo);
                }
            }
            fclose(verificar_pass);
        }

        if (assento_ocupado) {
            printf("\nTentativa abortada. Deseja tentar novamente?\n[1 para Sim / 0 para Sair]: ");
            scanf("%d", &opc);
            continue;
        }

        cad_passa.id = contador_id + 1;

        FILE *arquivo = fopen("voos.json", "r");
        FILE *temporario = fopen("temp.json", "w");
        int erro_vagas = 0;

        if (arquivo != NULL && temporario != NULL) {
            while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                cJSON *voo_json = cJSON_Parse(linha);
                if (voo_json != NULL) {
                    int id_atual = cJSON_GetObjectItem(voo_json, "id")->valueint;

                    if (id_atual == escolher_id) {
                        voo_encontrado = 1;
                        
                        if (cad_passa.classe == 1) { 
                            int cap_p = cJSON_GetObjectItem(voo_json, "cap_primeira")->valueint;
                            if (cap_p > 0) {
                                cJSON_ReplaceItemInObject(voo_json, "cap_primeira", cJSON_CreateNumber(cap_p - 1));
                            } else {
                                printf("\nAssentos insuficientes na Primeira Classe!");
                                erro_vagas = 1;
                            }
                        } else { 
                            int cap_e = cJSON_GetObjectItem(voo_json, "cap_economica")->valueint;
                            if (cap_e > 0) {
                                cJSON_ReplaceItemInObject(voo_json, "cap_economica", cJSON_CreateNumber(cap_e - 1));
                            } else {
                                printf("\nAssentos insuficientes na Classe Econômica!");
                                erro_vagas = 1;
                            }
                        }
                        
                        if (!erro_vagas) {
                            int cap_total = cJSON_GetObjectItem(voo_json, "capacidade")->valueint;
                            cJSON_ReplaceItemInObject(voo_json, "capacidade", cJSON_CreateNumber(cap_total - 1));
                        }
                    }
                    
                    char *texto_voo = cJSON_PrintUnformatted(voo_json);
                    fprintf(temporario, "%s\n", texto_voo);
                    free(texto_voo);
                    cJSON_Delete(voo_json);
                }
            }
            fclose(arquivo);
            fclose(temporario);

            if (erro_vagas) {
                remove("temp.json");
                printf("\nCadastro não efetuado por falta de vagas. Deseja tentar novamente?\n[1 para Sim / 0 para Sair]: ");
                scanf("%d", &opc);
                continue;
            }

            if (voo_encontrado) {
                remove("voos.json");
                rename("temp.json", "voos.json");
            } else {
                remove("temp.json");
                printf("\nErro: O ID do voo digitado não existe! Deseja tentar novamente?\n[1 para Sim / 0 para Sair]: ");
                scanf("%d", &opc);
                continue;
            }
        } else {
            printf("\nErro ao processar arquivo de voos.");
            if (arquivo) fclose(arquivo);
            if (temporario) fclose(temporario);
            return;
        }

        cJSON *pass_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(pass_json, "id", cad_passa.id);
        cJSON_AddNumberToObject(pass_json, "id_voo", escolher_id);
        cJSON_AddNumberToObject(pass_json, "classe", cad_passa.classe);
        cJSON_AddStringToObject(pass_json, "nome_passageiro", cad_passa.nome_passageiro);
        cJSON_AddStringToObject(pass_json, "cpf", cad_passa.cpf);
        cJSON_AddNumberToObject(pass_json, "assento", cad_passa.assento); 

        char *texto_JSON = cJSON_PrintUnformatted(pass_json);
        FILE *arq_pass = fopen("passageiros.json", "a");
        if (arq_pass != NULL) {
            fprintf(arq_pass, "%s\n", texto_JSON);
            fflush(arq_pass);
            fclose(arq_pass);
            printf("\nPassageiro cadastrado com sucesso! ID do Passageiro: %d\n", cad_passa.id);
        } else {
            printf("\nErro ao salvar arquivo de passageiros.");
        }

        free(texto_JSON);
        cJSON_Delete(pass_json);

        printf("\nDeseja cadastrar outro passageiro?\n[1 para Sim / 0 para Cancelar]: ");
        scanf("%d", &opc);
    } while(opc != 0);
}

void confirmar_voo() {
    char linha[MAX_READ];
    char cpf_digitado[MAX];
    int id_voo_pesquisa;
    int voo_tem_passageiros;
    int faltas_primeira;
    int faltas_economica;

    do {
        printf("\n--------Confirmar voo---------\n");
        printf("Digite o ID do voo para iniciar as confirmações\n");
        printf("(ou digite 0 para voltar ao menu principal): ");
        scanf("%d", &id_voo_pesquisa);

        if (id_voo_pesquisa == 0) {
            break;
        }

        voo_tem_passageiros = 0;
        faltas_primeira = 0;
        faltas_economica = 0;

        FILE *arquivo_pass = fopen("passageiros.json", "r");
        FILE *temp_pass = fopen("temp_pass.json", "w");
        FILE *arq_confirmados = fopen("confirmados.json", "a");

        if (arquivo_pass != NULL && temp_pass != NULL && arq_confirmados != NULL) {
            while (fgets(linha, sizeof(linha), arquivo_pass) != NULL) {
                cJSON *pass_json = cJSON_Parse(linha);
                if (pass_json != NULL) {
                    int voo_do_pass = cJSON_GetObjectItem(pass_json, "id_voo")->valueint;

                    if (voo_do_pass == id_voo_pesquisa) {
                        voo_tem_passageiros = 1;
                        char *nome_pass = cJSON_GetObjectItem(pass_json, "nome_passageiro")->valuestring;
                        char *cpf_salvo = cJSON_GetObjectItem(pass_json, "cpf")->valuestring;
                        int classe_pass = cJSON_GetObjectItem(pass_json, "classe")->valueint;

                        printf("\n----------------------------------------\n");
                        printf("Passageiro: %s\n", nome_pass);
                        printf("Digite o CPF para confirmar o embarque: ");
                        scanf(" %[^\n]", cpf_digitado);

                        if (strcmp(cpf_salvo, cpf_digitado) == 0) {
                            printf("Embarque CONFIRMADO com sucesso para %s!\n", nome_pass);
                            char *texto_confirmado = cJSON_PrintUnformatted(pass_json);
                            fprintf(arq_confirmados, "%s\n", texto_confirmado);
                            free(texto_confirmado);
                        } else {
                            printf("Erro: CPF não confere! Passageiro ausente/não confirmado.\n");
                            if (classe_pass == 1) {
                                faltas_primeira++;
                            } else {
                                faltas_economica++;
                            }
                            
                            char *texto_pendente = cJSON_PrintUnformatted(pass_json);
                            fprintf(temp_pass, "%s\n", texto_pendente);
                            free(texto_pendente);
                        }
                    } else {
                        char *texto_outro_voo = cJSON_PrintUnformatted(pass_json);
                        fprintf(temp_pass, "%s\n", texto_outro_voo);
                        free(texto_outro_voo);
                    }
                    cJSON_Delete(pass_json);
                }
            }
            fclose(arquivo_pass);
            fclose(temp_pass);
            fclose(arq_confirmados);

            remove("passageiros.json");
            rename("temp_pass.json", "passageiros.json");

            if (!voo_tem_passageiros) {
                printf("\nNenhum passageiro pendente encontrado para o voo ID %d.\n", id_voo_pesquisa);
            } else {
                printf("\nProcessamento de embarque finalizado para o voo ID %d.\n", id_voo_pesquisa);
                printf("Não comparecimentos - 1º Classe: %d | Classe Econômica: %d\n", faltas_primeira, faltas_economica);
                
                if (faltas_primeira > 0 || faltas_economica > 0) {
                    FILE *arq_voo = fopen("voos.json", "r");
                    FILE *temp_voo = fopen("temp_voos.json", "w");

                    if (arq_voo != NULL && temp_voo != NULL) {
                        while (fgets(linha, sizeof(linha), arq_voo) != NULL) {
                            cJSON *voo_json = cJSON_Parse(linha);
                            if (voo_json != NULL) {
                                int id_atual = cJSON_GetObjectItem(voo_json, "id")->valueint;

                                if (id_atual == id_voo_pesquisa) {
                                    int cap_p = cJSON_GetObjectItem(voo_json, "cap_primeira")->valueint;
                                    int cap_e = cJSON_GetObjectItem(voo_json, "cap_economica")->valueint;
                                    int cap_t = cJSON_GetObjectItem(voo_json, "capacidade")->valueint;

                                    cJSON_ReplaceItemInObject(voo_json, "cap_primeira", cJSON_CreateNumber(cap_p + faltas_primeira));
                                    cJSON_ReplaceItemInObject(voo_json, "cap_economica", cJSON_CreateNumber(cap_e + faltas_economica));
                                    cJSON_ReplaceItemInObject(voo_json, "capacidade", cJSON_CreateNumber(cap_t + faltas_primeira + faltas_economica));
                                    
                                    printf("\n[OVERBOOKING CALCULADO] Vagas liberadas devolvidas ao sistema de voos!\n");
                                }

                                char *texto_voo = cJSON_PrintUnformatted(voo_json);
                                fprintf(temp_voo, "%s\n", texto_voo);
                                free(texto_voo);
                                cJSON_Delete(voo_json);
                            }
                        }
                        fclose(arq_voo);
                        fclose(temp_voo);

                        remove("voos.json");
                        rename("temp_voos.json", "voos.json");
                    } else {
                        if (arq_voo) fclose(arq_voo);
                        if (temp_voo) fclose(temp_voo);
                    }

                    FILE *limpar_pass = fopen("passageiros.json", "r");
                    FILE *temp_limpeza = fopen("temp_limpeza.json", "w");
                    if (limpar_pass != NULL && temp_limpeza != NULL) {
                        while (fgets(linha, sizeof(linha), limpar_pass) != NULL) {
                            cJSON *pass_json = cJSON_Parse(linha);
                            if (pass_json != NULL) {
                                int voo_do_pass = cJSON_GetObjectItem(pass_json, "id_voo")->valueint;
                                if (voo_do_pass != id_voo_pesquisa) {
                                    char *texto_manter = cJSON_PrintUnformatted(pass_json);
                                    fprintf(temp_limpeza, "%s\n", texto_manter);
                                    free(texto_manter);
                                }
                                cJSON_Delete(pass_json);
                            }
                        }
                        fclose(limpar_pass);
                        fclose(temp_limpeza);
                        remove("passageiros.json");
                        rename("temp_limpeza.json", "passageiros.json");
                    } else {
                        if (limpar_pass) fclose(limpar_pass);
                        if (temp_limpeza) fclose(temp_limpeza);
                    }
                }
                printf("\nPressione ENTER para continuar...\n");
                getchar(); getchar();
            }
        } else {
            printf("\nErro ao processar arquivos do sistema.\n");
            if (arquivo_pass) fclose(arquivo_pass);
            if (temp_pass) fclose(temp_pass);
            if (arq_confirmados) fclose(arq_confirmados);
            break;
        }


    } while (id_voo_pesquisa != 0);
}

void editar_voo() {
    char linha[MAX_READ];
    int id_pesquisa;
    int voo_encontrado = 0;
    int opcao_edicao;
    
    char novo_nome[MAX];
    char novo_destino[MAX_READ];
    char novo_horario[6];
    unsigned int nova_capacidade;

    printf("\n----------Editar Voo----------\n");
    printf("Digite o ID do voo que deseja editar: ");
    scanf("%d", &id_pesquisa);

    FILE *arquivo = fopen("voos.json", "r");
    FILE *temporario = fopen("temp_voos.json", "w");

    if (arquivo != NULL && temporario != NULL) {
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            cJSON *voo_json = cJSON_Parse(linha);
            if (voo_json != NULL) {
                int id_atual = cJSON_GetObjectItem(voo_json, "id")->valueint;

                if (id_atual == id_pesquisa) {
                    voo_encontrado = 1;
                    
                    char *nome_atual = cJSON_GetObjectItem(voo_json, "nome_voo")->valuestring;
                    char *destino_atual = cJSON_GetObjectItem(voo_json, "destino")->valuestring;
                    char *horario_atual = cJSON_GetObjectItem(voo_json, "horário")->valuestring;
                    int cap_total_atual = cJSON_GetObjectItem(voo_json, "capacidade")->valueint;

                    printf("\nVoo Encontrado: %s | Destino: %s | Horário: %s | Capacidade: %d\n", 
                           nome_atual, destino_atual, horario_atual, cap_total_atual);
                    printf("[1] Alterar Nome do Voo\n");
                    printf("[2] Alterar Destino\n");
                    printf("[3] Alterar Horário\n");
                    printf("[4] Alterar Capacidade Total\n");
                    printf("[5] Alterar Preço da Classe Económica\n");
                    printf("[6] Alterar Preço da Primeira Classe\n");
                    printf("Escolha a opção que deseja editar: ");
                    scanf("%d", &opcao_edicao);

                    switch (opcao_edicao) {
                        case 1:
                            printf("Digite o novo nome do voo: ");
                            scanf(" %[^\n]", novo_nome);
                            cJSON_ReplaceItemInObject(voo_json, "nome_voo", cJSON_CreateString(novo_nome));
                            printf("\nNome do voo alterado com sucesso!");
                            break;

                        case 2:
                            printf("Digite o novo destino da viagem: ");
                            scanf(" %[^\n]", novo_destino);
                            cJSON_ReplaceItemInObject(voo_json, "destino", cJSON_CreateString(novo_destino));
                            printf("\nDestino alterado com sucesso!");
                            break;

                        case 3:
                            printf("Digite o novo horário (ex: 14:30): ");
                            scanf(" %[^\n]", novo_horario);
                            cJSON_ReplaceItemInObject(voo_json, "horário", cJSON_CreateString(novo_horario));
                            printf("\nHorário alterado com sucesso!");
                            break;

                        case 4:
                            printf("Digite a nova capacidade total do voo: ");
                            scanf("%u", &nova_capacidade);

                            int cap_p_atual = cJSON_GetObjectItem(voo_json, "cap_primeira")->valueint;
                            int cap_e_atual = cJSON_GetObjectItem(voo_json, "cap_economica")->valueint;

                            int vendidos_primeira = (cap_total_atual / 2) - cap_p_atual;
                            int vendidos_economica = (cap_total_atual - (cap_total_atual / 2)) - cap_e_atual;

                            int nova_cap_p = (nova_capacidade / 2) - vendidos_primeira;
                            int nova_cap_e = (nova_capacidade - (nova_capacidade / 2)) - vendidos_economica;

                            if (nova_cap_p < 0 || nova_cap_e < 0) {
                                printf("\nErro: A nova capacidade é menor do que a quantidade de passagens já vendidas!");
                            } else {
                                cJSON_ReplaceItemInObject(voo_json, "capacidade", cJSON_CreateNumber(nova_capacidade));
                                cJSON_ReplaceItemInObject(voo_json, "cap_primeira", cJSON_CreateNumber(nova_cap_p));
                                cJSON_ReplaceItemInObject(voo_json, "cap_economica", cJSON_CreateNumber(nova_cap_e));
                                printf("\nCapacidade alterada e redistribuída com sucesso!");
                            }
                            break;
                            case 5:
                                printf("Digite o novo preço da classe económica: ");
                                float novo_p_econ;
                                scanf("%f", &novo_p_econ);
                                cJSON_ReplaceItemInObject(voo_json, "preco", cJSON_CreateNumber(novo_p_econ));
                                printf("\nPreço da classe económica alterado com sucesso!");
                            break;

                            case 6:
                                printf("Digite o novo preço da primeira classe: ");
                                float novo_p_prim;
                                scanf("%f", &novo_p_prim);
                                cJSON_ReplaceItemInObject(voo_json, "preco_primeira_classe", cJSON_CreateNumber(novo_p_prim));
                                printf("\nPreço da primeira classe alterado com sucesso!");
                                break;

                        default:
                            printf("\nOpção inválida! Nenhuma alteração foi feita.");
                            break;
                    }
                }

                char *texto_voo = cJSON_PrintUnformatted(voo_json);
                fprintf(temporario, "%s\n", texto_voo);
                free(texto_voo);
                cJSON_Delete(voo_json);
            }
        }
        fclose(arquivo);
        fclose(temporario);

        if (voo_encontrado) {
            remove("voos.json");
            rename("temp_voos.json", "voos.json");
        } else {
            remove("temp_voos.json");
            printf("\nErro: O ID do voo digitado não existe!");
        }
    } else {
        printf("\nErro ao processar arquivo de voos.");
        if (arquivo) fclose(arquivo);
        if (temporario) fclose(temporario);
    }
}

void editar_passageiro() {
    char linha[MAX_READ];
    int id_pesquisa;
    int passageiro_encontrado = 0;
    int opcao_edicao;

    char novo_nome[MAX];
    char novo_cpf[MAX];
    int nova_classe;
    unsigned int novo_assento;

    printf("\n----------Editar Passageiro----------\n");
    printf("Digite o ID do passageiro que deseja editar: ");
    scanf("%d", &id_pesquisa);

    FILE *arquivo = fopen("passageiros.json", "r");
    FILE *temporario = fopen("temp_pass.json", "w");

    if (arquivo != NULL && temporario != NULL) {
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            cJSON *pass_json = cJSON_Parse(linha);
            if (pass_json != NULL) {
                int id_atual = cJSON_GetObjectItem(pass_json, "id")->valueint;

                if (id_atual == id_pesquisa) {
                    passageiro_encontrado = 1;

                    char *nome_atual = cJSON_GetObjectItem(pass_json, "nome_passageiro")->valuestring;
                    char *cpf_atual = cJSON_GetObjectItem(pass_json, "cpf")->valuestring;
                    int classe_atual = cJSON_GetObjectItem(pass_json, "classe")->valueint;
                    unsigned int assento_atual = (unsigned int)cJSON_GetObjectItem(pass_json, "assento")->valueint;
                    int id_voo = cJSON_GetObjectItem(pass_json, "id_voo")->valueint;

                    printf("\nPassageiro Encontrado: %s | CPF: %s | Classe: %s | Assento: %u\n",
                           nome_atual, cpf_atual, (classe_atual == 1 ? "Primeira" : "Economica"), assento_atual);
                    printf("[1] Alterar Nome\n");
                    printf("[2] Alterar CPF\n");
                    printf("[3] Alterar Classe e Assento\n");
                    printf("Escolha a opção que deseja editar: ");
                    scanf("%d", &opcao_edicao);

                    switch (opcao_edicao) {
                        case 1:
                            printf("Digite o novo nome: ");
                            scanf(" %[^\n]", novo_nome);
                            cJSON_ReplaceItemInObject(pass_json, "nome_passageiro", cJSON_CreateString(novo_nome));
                            printf("\nNome alterado com sucesso!");
                            break;

                        case 2:
                            printf("Digite o novo CPF: ");
                            scanf(" %[^\n]", novo_cpf);
                            cJSON_ReplaceItemInObject(pass_json, "cpf", cJSON_CreateString(novo_cpf));
                            printf("\nCPF alterado com sucesso!");
                            break;

                        case 3:
                            printf("Digite a nova classe ([1] Primeira | [0] Economica): ");
                            scanf("%d", &nova_classe);
                            printf("Digite o número do novo assento: ");
                            scanf("%u", &novo_assento);

                            int assento_ocupado = 0;
                            FILE *verificar_assento = fopen("passageiros.json", "r");
                            if (verificar_assento != NULL) {
                                char linha_v[MAX_READ];
                                while (fgets(linha_v, sizeof(linha_v), verificar_assento) != NULL) {
                                    cJSON *pass_v = cJSON_Parse(linha_v);
                                    if (pass_v != NULL) {
                                        int id_v = cJSON_GetObjectItem(pass_v, "id")->valueint;
                                        int voo_v = cJSON_GetObjectItem(pass_v, "id_voo")->valueint;
                                        int classe_v = cJSON_GetObjectItem(pass_v, "classe")->valueint;
                                        unsigned int assento_v = (unsigned int)cJSON_GetObjectItem(pass_v, "assento")->valueint;

                                        if (id_v != id_pesquisa && voo_v == id_voo && classe_v == nova_classe && assento_v == novo_assento) {
                                            assento_ocupado = 1;
                                        }
                                        cJSON_Delete(pass_v);
                                    }
                                }
                                fclose(verificar_assento);
                            }

                            if (assento_ocupado) {
                                printf("\nErro: O assento %u já está ocupado nesta classe para este voo! Alteração abortada.", novo_assento);
                                break;
                            }

                            FILE *arq_voo = fopen("voos.json", "r");
                            FILE *temp_voo = fopen("temp_voos.json", "w");
                            int erro_vagas = 0;

                            if (arq_voo != NULL && temp_voo != NULL) {
                                char linha_voo[MAX_READ];
                                while (fgets(linha_voo, sizeof(linha_voo), arq_voo) != NULL) {
                                    cJSON *voo_json = cJSON_Parse(linha_voo);
                                    if (voo_json != NULL) {
                                        int id_v_atual = cJSON_GetObjectItem(voo_json, "id")->valueint;

                                        if (id_v_atual == id_voo) {
                                            int cap_p = cJSON_GetObjectItem(voo_json, "cap_primeira")->valueint;
                                            int cap_e = cJSON_GetObjectItem(voo_json, "cap_economica")->valueint;

                                            if (classe_atual == 1 && nova_classe == 0) {
                                                if (cap_e > 0) {
                                                    cJSON_ReplaceItemInObject(voo_json, "cap_primeira", cJSON_CreateNumber(cap_p + 1));
                                                    cJSON_ReplaceItemInObject(voo_json, "cap_economica", cJSON_CreateNumber(cap_e - 1));
                                                } else {
                                                    erro_vagas = 1;
                                                }
                                            } else if (classe_atual == 0 && nova_classe == 1) {
                                                if (cap_p > 0) {
                                                    cJSON_ReplaceItemInObject(voo_json, "cap_primeira", cJSON_CreateNumber(cap_p - 1));
                                                    cJSON_ReplaceItemInObject(voo_json, "cap_economica", cJSON_CreateNumber(cap_e + 1));
                                                } else {
                                                    erro_vagas = 1;
                                                }
                                            }
                                        }
                                        char *texto_v = cJSON_PrintUnformatted(voo_json);
                                        fprintf(temp_voo, "%s\n", texto_v);
                                        free(texto_v);
                                        cJSON_Delete(voo_json);
                                    }
                                }
                                fclose(arq_voo);
                                fclose(temp_voo);

                                if (erro_vagas) {
                                    printf("\nErro: Não há assentos disponíveis na classe desejada para efetuar a troca!");
                                    remove("temp_voos.json");
                                } else {
                                    remove("voos.json");
                                    rename("temp_voos.json", "voos.json");
                                    cJSON_ReplaceItemInObject(pass_json, "classe", cJSON_CreateNumber(nova_classe));
                                    cJSON_ReplaceItemInObject(pass_json, "assento", cJSON_CreateNumber(novo_assento));
                                    printf("\nClasse e assento modificados com sucesso!");
                                }
                            }
                            break;

                        default:
                            printf("\nOpção inválida! Nenhuma alteração foi feita.");
                            break;
                    }
                }

                char *texto_pass = cJSON_PrintUnformatted(pass_json);
                fprintf(temporario, "%s\n", texto_pass);
                free(texto_pass);
                cJSON_Delete(pass_json);
            }
        }
        fclose(arquivo);
        fclose(temporario);

        if (passageiro_encontrado) {
            remove("passageiros.json");
            rename("temp_pass.json", "passageiros.json");
        } else {
            remove("temp_pass.json");
            printf("\nErro: O ID do passageiro digitado não existe!");
        }
    } else {
        printf("\nErro ao acessar os arquivos do sistema.");
        if (arquivo) fclose(arquivo);
        if (temporario) fclose(temporario);
    }
}

int main(){
    int opc = 1;
    #ifdef _WIN32
    SetCurrentDirectory("C:\\Users\\endua\\Videos\\airport");
    #else
    system("pwd");
    #endif

    do {
        printf("\n-------Sistema de Aeroporto--------\n");
        printf("\n[1] Cadastrar voo");
        printf("\n[2] Voos cadastrados");
        printf("\n[3] Cadastrar passageiro");
        printf("\n[4] Confirmar voo");

        printf("\n------Sistema-------\n");
        printf("\n[5] Editar voo");
        printf("\n[6] Editar passageiro");

        printf("\n[0] para sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: cadastrar_voo(); break;
            case 2: historico_voo(); break;
            case 3: cadastrar_passageiro(); break;
            case 4: confirmar_voo(); break;
            case 5: editar_voo(); break;
            case 6: editar_passageiro(); break;
            case 0: printf("\nSaindo! Tenha um ótimo dia.") break 
            default: printf("\nOpção inválida ou não encontrada\n"); break;
        }
    } while (opc != 0);
   
    return 0;
}
