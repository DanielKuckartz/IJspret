from scipy.optimize import linprog

# Verwijdert zo veel mogelijk verboden velden uit een voorkeurenmatrix en 
#   retourneert een matrix die gebruikt kan worden voor linprog
def comprimeer (matrix):
	m = len(matrix) # constante
	cumu = [0] # het totaal aantal toegestane velden in de eerste t/m i-de rij
	n = 0 # nu een hulpvariabele
	for rij in matrix:
		for cel in rij:
			if (cel != 0):
				n += 1
		cumu.append(n)
	# n is nu het totale aantal toegestane velden
	hulp = [0] * m # aantal toegestane velden in rij i waarover we straks ge-itereerd hebben
	B = [] # B in Mat(2m, n), maar dan met alle onnodige variabelen eruit gefilterd
	for rij in range(m):
		B.append( [0] * cumu[rij] 
						+ [waarde for waarde in matrix[rij] if waarde != 0]
						+ [0] * ( n - cumu[rij+1] ) 
						)
	for kol in range(m):
		B.append ( [0]*n ) # de randvoorwaarde voor kolom kol
		for rij in range(m):
			if (matrix[rij][kol] != 0):
				B[ m+kol ][ cumu[rij] + hulp[rij] ] = matrix[rij][kol]
				hulp[rij] += 1
	return B

# Vertaalt een linprog optimale oplossing van een gecomprimeerde matrix naar de
#   oplossing van de oorspronkelijke matrix retourneert de bijbehorende 
#   'uitgepakte' oplossing. Het eerste argument is de oorspronkelijke matrix.
def uitpakken (matrix, oplossing):
	m = len(matrix)
	x = [] # de 'uitgepakte' oplossing
	it = 0 # de it-de coordinaat van de oplossingsvector
	for rij in range(m):
		for kol in range(m):
			if (matrix[rij][kol] != 0):
				if (oplossing[it] == matrix[rij][kol]):
					x.append(kol)
				it += 1
	return x

def opdr_b ():
	A =	[	[ 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0 ],
				[ 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 ],
				[ 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 ],
				[ 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1 ],
				[ 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1 ],
				[ 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 ],
				[ 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1 ],
				[ 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1 ],
				[ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1 ],
				[ 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 ],
				[ 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1 ],
				[ 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 ],
				[ 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1 ],
				[ 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 ],
				[ 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0 ]
			]
	B = comprimeer(A)
	p = [-1] * len(B[0]) # maximaliseer het aantal toegewezen ijsjes
								 		 #   Merk op: linprog() minimaliseert ipv maximaliseert, 
								 		 #   dus we optimliseren het tegenstelde van de doelfunctie
	b = [1] * len(B)  # Elk kind krijgt 0 tot 1 ijsjes (0 <= i < m)
						 			# 	 en elk ijsje wordt gegeten door 0 tot 1 kinderen (m <= i < 2m)
	res = linprog(p, A_ub=B, b_ub=b, bounds=(0,1))
	x = uitpakken(A, res.x)
	for ijsje in x:
		print( '0 '*(ijsje) + '1 ' + '0 '*(len(A)-ijsje-1) )

# Voert een matrix met n voorkeuren 1, 2, 3 over in een matrix met waarden
#   gelijk aan 1 en zet waarden in de doelvector p zodanig dat een betere 
#   voorkeur volledige voorrang heeft op een slechtere voorkeur. Argument n is 
#   het aantal verschillende voorkeuren (ongelijk aan 0)
def voorkeuren (matrix, n) :
	m = len(matrix)
	p = []
	waarde = -1
	transformatie = {0: 0}
	for i in range(n,0,-1):
		transformatie.update( {i: waarde} )
		waarde = waarde*m - 1

	for rij in range(m):
		for kol in range(m):
			if (matrix[rij][kol] != 0):
				p.append( transformatie[ matrix[rij][kol] ] )
				matrix[rij][kol] = 1
	return p

def opdr_c ():
	A = [ [ 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0 ],
				[ 0, 3, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0 ],
				[ 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 3, 0, 0 ],
				[ 1, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 ],
				[ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 3 ],
				[ 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0 ],
				[ 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ],
				[ 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 2 ],
				[ 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0 ],
				[ 0, 0, 2, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0 ],
				[ 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 2 ],
				[ 0, 0, 0, 3, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0 ],
				[ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 2 ],
				[ 0, 1, 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 ],
				[ 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0 ] 
			]
	m = len(A)
	p = voorkeuren(A, 3)
	B = comprimeer(A)
	# We weten a priori niet of het mogelijk is dat elk kind zijn eerste keuze
	#   krijgt. Het kan nu zijn dat kind 1 zijn eerste keuze krijgt en alle andere
	#   kinderen niets. Dus we moeten echt aangeven dat het om een permutatie gaat
	b = [1] * len(B) + [-1] * m # we hoeven alleen rijen gelijk aan 1 te stellen
	for rij in range(m):
		B.append( [-cel for cel in B[rij]] )
	res = linprog(p, A_ub=B, b_ub=b, bounds=(0,1))
	x = uitpakken(A, res.x)
	for ijsje in x:
		print( '0 '*(ijsje) + '1 ' + '0 '*(len(A)-ijsje-1) )

def main ():
	print("Deelopdracht b:")
	opdr_b()
	print("\nDeelopdracht c:")
	opdr_c()

main()
