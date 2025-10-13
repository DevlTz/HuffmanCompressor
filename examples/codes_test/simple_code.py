import pandas as pd
import numpy as np

def avaliar_posicao_cabeca(df_rep_angulos):
   
    resultados = {}
    for lado in ['esquerdo', 'direito']:
        coluna_angulo = f'angulo_pescoco_{lado}'
        if coluna_angulo not in df_rep_angulos.columns:
            resultados[f'cabeca_{lado}'] = 1 
            continue

        variacao = df_rep_angulos[coluna_angulo].max() - df_rep_angulos[coluna_angulo].min()
        
        if variacao > 10: 
            resultados[f'cabeca_{lado}'] = 1 
        else:
            resultados[f'cabeca_{lado}'] = 0
            
    return resultados