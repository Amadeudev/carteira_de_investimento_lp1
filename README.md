# 📈 Otimizador de Carteira de Ações

Projeto final da disciplina de **Linguagem de Programação I** que implementa uma solução em C para o "Problema de Otimização de Carteira de Ações", utilizando o algoritmo de Programação Dinâmica para garantir a solução ótima.

## 👨‍💻 Equipe

* **Aluno 1:** AMADEU FERNANDES DA SILVA NETO
* **Aluno 2:** LUIS FELIPE LOPES ROSA
* **Aluno 3:** YAN ROCHA SILVA ALVES

---
## 📝 Enunciado do Projeto

### 1. Objetivo
Desenvolver um programa em linguagem C que, a partir de um capital de investimento limitado e uma lista de ações disponíveis, determine a combinação de compra que maximiza o retorno anual esperado para o investidor.

### 2. Contexto do Problema
Uma consultoria de investimentos precisa de uma ferramenta computacional para otimizar a montagem de carteiras de ações para seus clientes. A ferramenta deve analisar um conjunto pré-selecionado de ativos, cada um com seu respectivo custo de aquisição e uma projeção de retorno anual. O desafio consiste em selecionar um subconjunto desses ativos de forma que a soma de seus retornos seja a maior possível, sem que o custo total para adquiri-los ultrapasse o capital que o cliente deseja investir. Para simplificar o modelo, para cada ação a decisão é binária: ou você a compra (1) ou não a compra (0).

### 3. Especificações Técnicas
* **Entradas do Programa:**
    * `Capital_Total`: Um valor decimal (`float` ou `double`) que representa o montante máximo a ser investido.
    * `Lista_de_Ações`: Lida a partir de um arquivo, contendo o custo (decimal) e o retorno anual esperado (decimal) para cada ação.
* **Saída Esperada:**
    * A lista de ações que compõem a carteira ótima.
    * O custo total da carteira selecionada.
    * O retorno total máximo que será obtido com essa combinação.

---
## ✨ Funcionalidades

* Leitura de dados de ações (sigla, valor, retorno e nome) a partir de um arquivo de texto (`dados_acoes.txt`).
* Interação com o usuário para obter o valor do capital de investimento.
* Alocação dinâmica de memória para lidar com um número variável de ações.
* Implementação do algoritmo de **Programação Dinâmica** para garantir a solução 100% ótima, resolvendo o "Problema da Mochila 0/1".
* Exibição clara da carteira de ações selecionada, com o custo total e o retorno máximo consolidado.
* Gerenciamento de memória adequado, liberando todos os recursos alocados ao final da execução.

---
## 🧠 Como Funciona: A Lógica do Algoritmo

Para garantir a melhor solução possível conforme o enunciado, a abordagem escolhida foi a **Programação Dinâmica**.

1.  **Discretização dos Custos:** O algoritmo de Programação Dinâmica para este problema requer que os pesos (custos) sejam inteiros. Para isso, todos os valores monetários (`capital` e `custo` das ações) são convertidos para centavos, multiplicando-os por 100 e arredondando para o inteiro mais próximo. Isso preserva a precisão enquanto permite a aplicação da técnica.

2.  **Construção da Tabela DP:** Uma matriz (`mochila`) é criada, onde cada célula `[i][c]` armazena o máximo retorno possível usando as primeiras `i` ações com um capital limite de `c` centavos. A tabela é preenchida sistematicamente, decidindo para cada ação se incluí-la ou não na carteira otimiza o retorno para aquele subproblema.

3.  **Reconstrução da Carteira (Backtracking):** Após a tabela ser totalmente preenchida, o valor na última célula (`mochila[nAcoes][capitalInt]`) representa o retorno máximo possível. O algoritmo então faz o caminho inverso, partindo desta última célula, para identificar exatamente quais ações foram incluídas na composição da carteira ótima.

Essa abordagem garante que o resultado encontrado seja sempre o melhor matematicamente possível.

---
## 🛠️ Como Compilar e Executar

Siga os passos abaixo para compilar e executar o projeto.

### Pré-requisitos

* Um compilador C (como o **GCC**) instalado no sistema.
* O arquivo de dados `dados_acoes.txt` presente na mesma pasta do executável com os dados de exemplo.

### Passo a Passo

1.  Salve o código-fonte como `main.c` em uma pasta.
2.  Na mesma pasta, crie o arquivo `dados_acoes.txt` com os dados das ações.
3.  Abra um terminal e navegue até a pasta do projeto.
4.  Compile o programa com o seguinte comando:
    ```bash
    gcc main.c -o otimizador
    ```
5.  Execute o programa compilado:
    * No **Linux ou macOS**: `./otimizador`
    * No **Windows**: `otimizador.exe`
6.  Siga as instruções no terminal para inserir o capital e ver o resultado.

---
## 📄 Formato do Arquivo de Entrada (`dados_acoes.txt`)

O programa espera que o arquivo `dados_acoes.txt` contenha uma ação por linha, com os dados separados por espaços, no seguinte formato:

`SIGLA VALOR RETORNO NOME_DA_EMPRESA`

* `SIGLA`: Ticker da ação (ex: A), sem espaços.
* `VALOR`: Custo da ação em Reais (ex: 2850.50).
* `RETORNO`: Retorno anual esperado em porcentagem (ex: 12.5).
* `NOME_DA_EMPRESA`: Nome da empresa. Use `_` para nomes compostos (ex: CLOUDTEC3).

### Exemplo Iniciante:
```
A 400.00 10.0 BIGCORP4
B 600.00 12.0 SAFETECH3
C 350.00 8.0 VALIOSA1
D 250.00 5.0 ESTAVEL5
```

### Exemplo Moderado:
```
A 2850.50 12.5 CLOUDTEC3
B 3500.00 14.0 IAGIGANTE4
C 1800.75 9.5 CYBERSEC1
D 4200.25 15.5 DATADRIVEN5
E 950.50 6.8 HARDWARE9
F 1500.00 8.0 GAMEDEV2
G 2100.80 10.2 FINTECH7
H 3100.00 13.0 BIOINFO6
I 1250.60 7.5 MOBILENET4
J 4800.00 16.0 QUANTUMSYS8
K 1650.40 8.8 SOCIALMEDIA3
L 2450.00 11.0 ROBOTICA1
M 1980.20 9.8 STREAMING5
N 2950.00 12.8 EDUTECH7
O 3800.90 14.5 SPACEXPLORE2
```
