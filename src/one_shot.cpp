#include "../include/Modele.h"
#include "../include/one_shot.h"

struct SoftModele 
{
    uint16_t nb_layers;
    void** modules;
    torch::Tensor (**activation)(torch::Tensor*, struct parafunc*);
    torch::Tensor (**forward_mod)(void*, torch::Tensor*);
};


torch::Tensor forward_linear(void* module, torch::Tensor* x) 
{
	std::cout << "Bienvenue dans la forward linear \n";
    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(module);
    std::cout << "apres cast \n";
    return lin->forward(*x);
    std::cout << "apres forward \n";
}



// allocation

void essaie_un_truc ()
{
	SoftModele M;
	M.nb_layers = 1;
	M.modules = new void*[M.nb_layers];

	// création d’un Linear
	auto* lin = new torch::nn::LinearImpl(torch::nn::LinearOptions(16, 32).bias(true));
	M.modules[0] = lin;

	// forward
	torch::Tensor x = torch::rand({1, 16});
	std::cout << "x = \n" << x << "\n";
	torch::Tensor y = forward_linear(M.modules[0], &x);
	std::cout << "y = \n" << y << "\n"; 
}
