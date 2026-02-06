import matplotlib.pyplot as plt

# Dati estratti dai tuoi log
threads = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
times = [0.0502693, 0.0474054, 0.0453521, 0.0436119, 0.0429084, 
         0.0444997, 0.0452828, 0.046339, 0.0465932, 0.0464618, 
         0.0475269, 0.0469856, 0.0459484, 0.0459006, 0.0474653]

# Creazione del grafico
plt.figure(figsize=(10, 6))
plt.plot(threads, times, marker='o', linestyle='-', color='b', linewidth=2)

# Evidenziamo il punto di minimo (ottimo locale)
min_time = min(times)
best_threads = threads[times.index(min_time)]
plt.annotate(f'Ottimo: {best_threads} thread\n{min_time:.5f}s', 
             xy=(best_threads, min_time), 
             xytext=(best_threads + 1, min_time + 0.0005))

# Formattazione
plt.title('Analisi Scalabilità OpenMP (Memory Bound Scenario)', fontsize=14)
plt.xlabel('Numero di Thread', fontsize=12)
plt.ylabel('Tempo di Esecuzione (secondi)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xticks(threads)

# Mostra grafico
plt.tight_layout()
plt.show()