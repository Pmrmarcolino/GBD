import math

# ----------------------Estatistica de Ordem----------------------
# Dado um conjunto A, de n elementos dar o i-esimo inteiro menor elemento de A

# Comparacees: n-1, nao sendo possivel fazer com menos
def minimo (A,n):
	min = A[0]
	for j in range(2,n):
		if min > A[j]:
			min = A[j]
	return min

#Comparacoes: O(n)
def minimax(A,n):
	min = A[0]
	max = A[0]
	for j in range(2,n):
		if A[j] < min:
			min = A[j]
		if A[j] > max:
			max = A[j]
	return (min,max)	

#Existe algortimos melhores para encontrar encontar o maior e menor elemento;

# ----------------------MergeSort----------------------

def intercala(A,p,q,r):
	B = [0] *len(A)
	for i in range(p,(q+1)):
		B[i] = A[i]
	for j in range(q+1,(r+1)):
		B[r+q+1-j] = A[j]

	i = p
	j = r

	for k in range (p,(r+1)):
		if(B[i] <= B[j]):
			A[k] = B[i]
			i = i+1
		else:
			A[k] = B[j]
			j = j-1

def merge(A):
	mergeSort(A,0,len(A)-1)

	return A

def mergeSort(A,p,r):
	if(p<r ):
		q = int(math.floor((p+r)/2))
		mergeSort(A,p,q)
		mergeSort(A,q+1,r)
		intercala(A,p,q,r)
	return A	

# ----------------------Primeira solucao----------------------
# Recebe-se A, e i 1<=i<=n e devolve o valor do i-esimo menor elemento de A

def Seleciona_Ord(A,i):  
	merge(A) # pode ser usado qualquer algoritmo de ordecoo, com complexidade de tempo O(nlog(n))
	return A[i-1]	


# ----------------------Primeira solucao----------------------
#Usa-se o particiona, que devolde um elemento k, tal que 1<=k<=n


def Particione(A,p,r):
	x = A[r]
	i = p-1
	troca = 0
	for j in range(p, (r-1)):
		if A[j] <= x:
			i +=1
			troca = A[i]
			A[i] = A[j]
			A[j] = troca
	troca = A[i+1] 		
	A[i+1] = A[r]
	A[r] =  troca
	return (i+1)		 
#Complexidade de tempo = teta(n²)
def Selecione_Particao(A,p,r,i): 
	if p == r:
		return A[p]
	q = Particione(A,p,r)
	k = q-p+1
	if i == k: # entao A[k] e o i-esimo menor
		return (A[k])
	else:
		if i < k: # entao o i-esimo menor esta a direita de k
			return Selecione_Particao(A, p, (q-1),i)
		else: # se nao o i-esimo menor esta a esquerda de k
			return Selecione_Particao(A, (q+1),r,i)	

def main():
	A = [22,33,55,77,99,11,44,66,88]
	p = 0
	r = len(A)-1
	i = 2 # i-esimo termo a ser encontrado
	print "Vetor desordenado: ", A

	print "\nMin:", minimo(A,r)

	print "\nMinmax", minimax(A,r)

	print "\nVetor ordenado: ", merge(A)

	print "\nSeleciona_ord de A: ", Seleciona_Ord(A,i)

	#print Particione(A,p,r)
	print "\nSeleciona_Particao de A: ", Selecione_Particao(A,p,r,i)

main()	