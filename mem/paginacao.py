tamanho_memoria = 64 * 1024  # 64 KB
tamanho_pagina = 4 * 1024    # 4 KB
num_paginas = tamanho_memoria // tamanho_pagina
memoria = [0] * num_paginas  # 0 = livre, 1 = ocupada

def alocar_paginacao(tamanho_processo):
    paginas_necessarias = (tamanho_processo + tamanho_pagina - 1) // tamanho_pagina
    alocadas = 0
    for i in range(num_paginas):
        if memoria[i] == 0:
            memoria[i] = 1
            alocadas += 1
            if alocadas == paginas_necessarias:
                print(f"Processo de {tamanho_processo} KB alocado em {paginas_necessarias} páginas.")
                return
    print(f"Memória insuficiente para alocar {tamanho_processo} KB.")

alocar_paginacao(5 * 1024)   # 5 KB (2 páginas, 3 KB desperdiçados)
alocar_paginacao(10 * 1024)  # 10 KB (3 páginas, 2 KB desperdiçados)
