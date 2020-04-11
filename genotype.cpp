#include <iostream>
#include <vector>
#include <string>

struct Gene {
	char allele_1;
	char allele_2;
};

using Genotype = std::vector<Gene>;
using str = std::string;

bool g_uniqueOnly{};
bool g_pack{};


void parseGenotype(str& str_genotype, Genotype& genotype) {
	genotype.resize(str_genotype.length() / 2);

	int gene_counter{};
	int str_index{};
	while (gene_counter < genotype.size()) {
		genotype[gene_counter].allele_1 = str_genotype[str_index++];
		genotype[gene_counter++].allele_2 = str_genotype[str_index++];
	}
}

void printGene(Gene& gene) {
	if (gene.allele_2 < gene.allele_1) {
		char allele_2 = gene.allele_1;
		gene.allele_1 = gene.allele_2;
		gene.allele_2 = allele_2;
	}

	std::cout << gene.allele_1 << gene.allele_2 << ' ';
}

void printGenotype(Genotype& genotype) {
	for (auto& gene : genotype)
		printGene(gene);

	std::cout << '\n';
}

Genotype getGenotype() {
	static bool number{};
	std::cout << "Please enter genotype of parent " << '#' << (number ? '2' : '1') << " >>> ";
	str str_genotype{};
	Genotype genotype;

	std::cin >> str_genotype;
	number = true;
	parseGenotype(str_genotype, genotype);
	return genotype;
}

Genotype randomizeGenes(Gene& one, Gene& two) {
	Genotype pack_o_genes(4);

	pack_o_genes[0].allele_1 = one.allele_1;
	pack_o_genes[0].allele_2 = two.allele_2;

	pack_o_genes[1].allele_1 = one.allele_1;
	pack_o_genes[1].allele_2 = two.allele_1;

	pack_o_genes[2].allele_1 = one.allele_2;
	pack_o_genes[2].allele_2 = two.allele_2;

	pack_o_genes[3].allele_1 = one.allele_2;
	pack_o_genes[3].allele_2 = two.allele_1;

	return pack_o_genes;
}

bool isUnique(Gene& gene, Genotype& unique_genes) {
	for (auto& unique_gene : unique_genes)
		if (gene.allele_1 == unique_gene.allele_1) {
			if (gene.allele_2 == unique_gene.allele_2)
				return false;
		}
		else if (gene.allele_1 == unique_gene.allele_2) {
			if (gene.allele_2 == unique_gene.allele_1)
				return false;
		}

	return true;
}

Genotype selectUnique(Genotype genes_list) {
	Genotype unique{ genes_list[0] };

	for (auto& gene : genes_list)
		if (isUnique(gene, unique))
			unique.push_back(gene);

	return unique;
}

str geneToStr(Gene& gene) {
	if (gene.allele_2 < gene.allele_1) {
		char allele_2 = gene.allele_1;
		gene.allele_1 = gene.allele_2;
		gene.allele_2 = allele_2;
	}

	std::string str{};
	str += gene.allele_1;
	str += gene.allele_2;
	return str;
}

bool isUniqueStr(str& genes, std::vector<str>& strings) {
	for (str& string : strings)
		if (genes == string)
			return false;

	return true;
}

std::vector<bool> decideBools(int& i, int size) {
	std::vector<bool> bools(size, false);

	if (!i)
		return bools;

anchor:
	for (int j{}; j < size; ++j) {
		if (!i)
			return bools;

		int true_index{};

		for (int k{ size }; k > 0; --k)
			if (i % k == 0) {
				true_index = k - 1;
				i -= k;
				break;
			}

		bools[true_index] = !bools[true_index];

		if (i < 0)
			return bools;
	}

	if (i)
		goto anchor;

	return bools;
}

char returnAllele(Gene& gene, bool isDominant) {
	if (isDominant)
		return (gene.allele_1 > gene.allele_2) ? gene.allele_2 : gene.allele_1;

	return (gene.allele_1 < gene.allele_2) ? gene.allele_2 : gene.allele_1;
}

str genesBuilder(Genotype& genotype, int& i) {
	int original_i{ i };
	std::vector<bool> bools{ decideBools(i, genotype.size()) };
	i = original_i;

	str genes{};

	for (int j{}; j < genotype.size(); ++j)
		genes += returnAllele(genotype[j], bools[j]);

	return genes;
}

std::vector<str> genotypeToStringies(Genotype& genotype) {
	std::vector<str> strings{};

	for (int i{}; i <= genotype.size() * genotype.size(); ++i) {
		str gene_str{ genesBuilder(genotype, i) };
		if (isUniqueStr(gene_str, strings))
			strings.push_back(gene_str);
	}

	return strings;
}

void print_str_Vector(std::vector<str>& strings) {
	for (str& i : strings)
		std::cout << i << ' ';

	std::cout << '\n';
}

Gene packGene(char allele_1, char allele_2) {
	if (allele_1 > allele_2)
		return { allele_2, allele_1 };
	else
		return { allele_1, allele_2 };
}

str breedStrings(str& genes1, str& genes2) {
	str resultBreed{};

	for (int i{}; i < genes1.size(); ++i) {
		Gene temp{ packGene(genes1[i], genes2[i]) };
		resultBreed += geneToStr(temp);
	}

	return resultBreed;
}

std::vector<str> breedGenotypes(std::vector<str>& strings1, std::vector<str>& strings2) {
	std::vector<str> breededStrings;
	
	for (int i{}; i < strings1.size(); ++i)
		for (int j{}; j < strings2.size(); ++j) {
			str genes{ breedStrings(strings1[i], strings2[j]) };
			if (g_uniqueOnly) {
				if (isUniqueStr(genes, breededStrings))
					breededStrings.push_back(genes);
			}
			else
				breededStrings.push_back(genes);
		}

	return breededStrings;
}

void createGeneration(Genotype& genotype1, Genotype& genotype2) {
	if (!g_pack) {
		std::vector<Genotype> new_generation(((genotype1.size() > genotype2.size()) ? genotype2.size() : genotype1.size()));
		for (int i{}; i < new_generation.size(); ++i) {
			new_generation[i] = randomizeGenes(genotype1[i], genotype2[i]);

			if (g_uniqueOnly) {
				Genotype temp{ selectUnique(new_generation[i]) };
				printGenotype(temp);
			}
			else
				printGenotype(new_generation[i]);
		}
	}

	if (g_pack) {
		std::vector<str> stringsOfGenotype1{ genotypeToStringies(genotype1) };
		std::vector<str> stringsOfGenotype2{ genotypeToStringies(genotype2) };
		std::vector<str> breeds{ breedGenotypes(stringsOfGenotype1, stringsOfGenotype2) };
		print_str_Vector(breeds);
	}
}

bool uniqueOnly() {
	char yn{};
	do {
		std::cout << "Would you like this program to show unique genes only? [y/n] >>> ";
		std::cin >> yn;
		std::cin.ignore(std::cin.rdbuf()->in_avail());
	} while (yn != 'n' && yn != 'y');

	return (yn == 'y') ? true : false;
}

bool pack() {
	char yn{};
	do {
		std::cout << "Would you like this program to pack genes into genotypes? [y/n] >>> ";
		std::cin >> yn;
		std::cin.ignore(std::cin.rdbuf()->in_avail());
	} while (yn != 'n' && yn != 'y');

	return (yn == 'y') ? true : false;
}

int main() {
	std::cout << "Welcome to Genotype Reader v1.4.\nCopyright: sirkostya009.\nLicense: Apache 2.0\n\n";
	g_uniqueOnly = uniqueOnly();
	g_pack = pack();

	Genotype genotype1{ getGenotype() };
	Genotype genotype2{ getGenotype() };

	std::cout << "\n";

	createGeneration(genotype1, genotype2);
	std::cout << "\nPress Enter to close this window . . .";
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::getchar();
}