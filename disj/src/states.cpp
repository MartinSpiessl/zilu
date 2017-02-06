#include "states.h"

States::~States() {
	if (values != NULL) {
		delete []values;
		values = NULL;
	}
	if (labels!= NULL) {
		delete []labels;
		labels = NULL;
	}
	if (t_index != NULL) {
		delete []t_index;
		t_index = NULL;
	}
}

bool States::initFromFile(int num, std::ifstream& fin) {
	int label;
	int tmpint;
	char tmpchar;
	for (int i = 0; i < num; i++) {
		fin >> label;
		for (int j = 0; j < Nv; j++) {
			fin >> tmpint >> tmpchar >> values[i][j];
			assert(tmpint == j);
			assert(tmpchar == ':');
		}
	}
	size += num;
	t_index[p_index + 1] = t_index[p_index] + num;
	p_index++;
	return true;
}

int States::addStates(State st[], int lb[], int len) {
	if (size + len >= max_size) {
		std::cerr << "Resizing..." << std::endl;
		int previous_max_size = max_size;
		double(*previous_values)[Nv] = values;
		max_size *= 2;

		if ((values = new double[max_size][Nv]) == NULL)
			return -1;
		memcpy(values, previous_values, previous_max_size * Nv * sizeof(double));

		int *previous_labels = labels;
		if ((labels = new int[max_size]) == NULL)
			return -1;
		memcpy(labels, previous_labels, previous_max_size * sizeof(int));

		int* previous_t_index = t_index;
		if ((t_index = new int[max_size]) == NULL)
			return -1;
		memcpy(t_index, previous_t_index, previous_max_size * sizeof(int));
	}
	//return 0;

	//std::cout << "58 in add states\n";
	int addLength = 0;
	for (int i = 0; i < len; i++) {
		// try to insert state st[i]
		bool skip = false;
		for (int j = 0; j < size; j++) {
			if ((stateCmp(values[j], st[i]) == true) && (labels[j] == lb[i])) {
				skip = true;
				break;
			}
		}
		if (skip) continue;
		stateCpy(&values[size], &st[i]);
		labels[size] = lb[i];
		if (lb[i] < Nb && lb[i] >= 0) size_b[lb[i]]++;
		//if (lb[i] >= 0) size_b[lb[i]]++;
		addLength++;
		size++;
	}
	//std::cout << "77 in add states p_index=" << p_index << "\n";
	//t_index[p_index + 1] = t_index[p_index] + addLength;
	//std::cout << "79 in add states p_index=" << p_index << "\n";
	p_index++;
	//std::cout << "+" << addLength << " ";
	return addLength;
}

void States::dumpTrace(int num) {
	if (num >= p_index) {
		std::cerr << "exceed state set boundary" << std::endl;
		return;
	}
	for (int i = t_index[num]; i < t_index[num + 1]; i++) {
		std::cout << BOLD << "[b" << labels[i] << "]" << NORMAL << "(" << values[i][0];
		for (int j = 1; j < Nv; j++)
			std::cout << "," << values[i][j];
		std::cout << ")->";
	}
	std::cout << "LABEL" << labelString(this->label);
}

std::ostream& operator<< (std::ostream& out, const States& ss) {
	//std::cout << "lable[" << ss.label << "]:" << std::endl;
	for (int i = 0; i < ss.p_index; i++) {
		std::cout << "\tTr." << i << ":";
		for (int j = ss.t_index[i]; j < ss.t_index[i + 1]; j++) {
			std::cout << BOLD << "[b" << ss.labels[j] << "]" << NORMAL << "(" << ss.values[j][0];
			for (int k = 1; k < Nv; k++)
				std::cout << "," << ss.values[j][k];
			std::cout << ")->";
		}
		std::cout << "LABLE" << labelString(ss.label) << std::endl;
	}
	return out;
}
