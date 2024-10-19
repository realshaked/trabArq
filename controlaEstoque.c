#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//integrantes: Henrique

/*detalhes: arquivo binário deve ser acessado com os modos de leitura e escrita (rb+),
e os produtos devem ser gravados e lidos utilizando as funções fwrite e fread, respectivamente.

Para a busca por código use fseek para acessar diretamente a posição correta do produto no arquivo.
Para a busca por descrição e para o relatório de produtos com estoque abaixo do mínimo, será necessário 
percorrer o arquivo sequencialmente.

Garanta o tratamento de erros, como tentativas de vender uma quantidade superior à disponível no estoque,
ou busca por código inexistente.*/

struct oProduto {
char descricao[40];
int quantidadeEstoque;
int minimoEstoque;
float precoVenda;
}; typedef struct oProduto Produto;

void incluirProduto(FILE *p)
{
Produto pinc;

printf("\nDigite a descricao do produto: ");
scanf("%s", &pinc.descricao);

printf("\nDigite a quantidade no estoque presente desse produto: ");
scanf("%d", &pinc.quantidadeEstoque);

printf("\nDigite o minimo necessario no estoque desse produto: ");
scanf("%d", &pinc.minimoEstoque);

printf("\nDigite o preco para venda desse produto: ");
scanf("%f", &pinc.precoVenda);

fwrite(&pinc, sizeof(Produto), 1, p);
}

void registraVenda(FILE *p)
{
    int cod = 0, qtdVendida, ir, suc;
    Produto preg;

    printf("\ninforme o codigo do produto: ");
    scanf("%d", &cod);
    fseek(p, cod, SEEK_SET);
    
    fread(&preg, sizeof(Produto), 1, p);
    printf("informe a quantidade vendida desse produto: ");
    scanf("%d", &qtdVendida);
    if(preg.quantidadeEstoque >= qtdVendida)
    {
        preg.quantidadeEstoque = preg.quantidadeEstoque - qtdVendida;
    }
    fwrite(&preg, sizeof(Produto), 1, p);
}

void buscaCodigo(FILE *p)
{
    int codigo;
    Produto pcod;
    printf("Digite o codigo do produto: ");
    scanf("%d", &codigo);
    fseek(p, codigo, SEEK_SET);
    fread(&pcod, sizeof(Produto), 1, p);
    printf("%s\t%d\t\t%d\t%.2f\n", pcod.descricao, pcod.quantidadeEstoque, pcod.minimoEstoque, pcod.precoVenda);
}

void buscaDescricao(FILE *p)
{
    char descricao[40];
    Produto pdesc;
    size_t resultado;
    rewind(p);
    printf("Digite a descricao do produto a ser procurada: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0;
    while((resultado = fread(&pdesc, sizeof(Produto), 1, p)) == 1)
    {
        if (strcmp(pdesc.descricao, descricao) == 0) {
            printf("%s\t%d\t\t%d\t%.2f\n", pdesc.descricao, pdesc.quantidadeEstoque, pdesc.minimoEstoque, pdesc.precoVenda);
        }
        if (strstr(pdesc.descricao, descricao) != NULL) {
            printf("%s\t%d\t\t%d\t%.2f\n", pdesc.descricao, pdesc.quantidadeEstoque, pdesc.minimoEstoque, pdesc.precoVenda);
        }
     
    }

}

void exibeRelatorio(FILE *p)
{
    Produto prel;
    int resultado;
    rewind(p);
    while((resultado = fread(&prel, sizeof(Produto), 1, p)) == 1)
    {
        if(prel.quantidadeEstoque < prel.minimoEstoque)
        {
            printf("%s\t%d\t\t%d\t%.2f\n", prel.descricao, prel.quantidadeEstoque, prel.minimoEstoque, prel.precoVenda); 
        }
    }

}

int main(int argc, char * argv[])
{
    FILE *arquivo;

    int op;
    arquivo = fopen("estoque.bin", "rb+");
    if(!arquivo)
    {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    printf("Escolha uma das opcoes:\n1. Incluir Produto\n2. Registrar Venda\n3. Buscar por Codigo\n4. Buscar por Descricao\n5. Relatorio de Produtos com Estoque Abaixo do Minimo\n6. Sair: ");
    scanf("%i", &op);
    switch(op)
    {
        case 1: //incluir produto
        incluirProduto(arquivo);
        break;
        case 2: //registrar venda
        registraVenda(arquivo);
        break;
        case 3: //buscar por codigo
        buscaCodigo(arquivo);
        break;
        case 4: //buscar por descricao
        buscaDescricao(arquivo);
        break;
        case 5: //relatorio de produtos com estoque abaixo do minimo
        exibeRelatorio(arquivo);
        break;
        case 6: //sair
        exit(0);
        break;
    }
    fclose(arquivo);
    return 0;
}