// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation
// à partir d'une DLL plus simple. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole ULTRAFLUIDMODELER_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini pour un projet
// qui utilise cette DLL. Ainsi, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions
// ULTRAFLUIDMODELER_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#ifdef ULTRAFLUIDMODELER_EXPORTS
#define ULTRAFLUIDMODELER_API __declspec(dllexport)
#else
#define ULTRAFLUIDMODELER_API __declspec(dllimport)
#endif

// Cette classe est exportée à partir de la dll
class ULTRAFLUIDMODELER_API CUltrafluidModeler {
public:
	CUltrafluidModeler(void);
	// TODO: ajoutez ici vos méthodes.
};

extern ULTRAFLUIDMODELER_API int nUltrafluidModeler;

ULTRAFLUIDMODELER_API int fnUltrafluidModeler(void);
