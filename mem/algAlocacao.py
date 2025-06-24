def first_fit(blocos_livres, tamanho):
    for i, bloco in enumerate(blocos_livres):
        if bloco >= tamanho:
            blocos_livres[i] -= tamanho
            print(f"First-Fit: Alocado {tamanho} KB no bloco {i}.")
            return
    print(f"First-Fit: Não há espaço para {tamanho} KB.")

def best_fit(blocos_livres, tamanho):
    melhor_idx = -1
    menor_sobra = float('inf')
    for i, bloco in enumerate(blocos_livres):
        if bloco >= tamanho and (bloco - tamanho) < menor_sobra:
            melhor_idx = i
            menor_sobra = bloco - tamanho
    if melhor_idx != -1:
        blocos_livres[melhor_idx] -= tamanho
        print(f"Best-Fit: Alocado {tamanho} KB no bloco {melhor_idx}.")
    else:
        print(f"Best-Fit: Não há espaço para {tamanho} KB.")

def worst_fit(blocos_livres, tamanho):
    maior_bloco = max(blocos_livres) if blocos_livres else 0
    if maior_bloco >= tamanho:
        idx = blocos_livres.index(maior_bloco)
        blocos_livres[idx] -= tamanho
        print(f"Worst-Fit: Alocado {tamanho} KB no maior bloco ({idx}).")
    else:
        print(f"Worst-Fit: Não há espaço para {tamanho} KB.")

blocos = [20, 10, 30]
first_fit(blocos.copy(), 15)
best_fit(blocos.copy(), 15)
worst_fit(blocos.copy(), 15)
