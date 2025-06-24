memoria_segmentacao = [0] * (64 * 1024)  # 64 KB

def alocar_segmentacao(tamanho, id_processo):
    inicio = 0
    while inicio <= len(memoria_segmentacao) - tamanho:
        if all(m == 0 for m in memoria_segmentacao[inicio:inicio + tamanho]):
            memoria_segmentacao[inicio:inicio + tamanho] = [id_processo] * tamanho
            print(f"Segmento de {tamanho} KB alocado no endereço {inicio}.")
            return
        inicio += 1
    print(f"Não foi possível alocar {tamanho} KB (fragmentação externa).")

alocar_segmentacao(8 * 1024, 1)   # Aloca 8 KB
alocar_segmentacao(12 * 1024, 2)   # Aloca 12 KB
alocar_segmentacao(10 * 1024, 3)   # Falha se não houver espaço contíguo
