NAME = libft.a				# designe la bibli statique libft.a
					# permet reutiliser le makefile pour different projet
CC = cc					# on declare plusieurs variables pour plus de flexibilite

CFLAGS = -Wall -Wextra -Werror		# si on veut changer les flags plus tard, pas besoins de reecrire tout par ex

AR = ar rcs 				# ar = archiveur
					# programme unix qui regroupe plusieur fichier .o dans un bibli statique .a
					# r -> replace/add : ajoute ou remplace fichier
					# c -> create : cree l'archive
					# s -> met a jour l'index de l'archive
					# va permettre de transformer tout les fichiers objets creer a partir des sources en une bibli.a
					# ca c est primordial pour la comprehension du makefile

SRCS = ft_vanikertonchien.c \		# \ indique que la variable continue a la ligne suivante		
	ft_vanikertamer.c \		# on declare toute les fonctions de not projet
	ft_va nikertonchat.c
		
OBJS = $(SRCS:.c=.o)			# on la variable OBJS (objets) egale toutes les varibles SRCS (sources) transforme de .c en .o
					# autrement dit on compile tous les fonction .c en objets .o

all: $(NAME)				# ainsi quand on fait make all on compile tout libft.a

$(NAME): $(OBJS) 			# la on dit : name est dependant de objects
	$(AR) $(NAME) $(OBJS)		# tu archives tout les object en une libt.a	
					# pierre angulaire de ce makefile

%.o: %.c libft.h			# Pour fabriquer n'importe quel fichier .o, j'ai besoin du fichier .c correspondant et de libft.h
	$(CC) $(CFLAGS) -c $< -o $@	# autrement dit %.o (nimporte quel fichier finisant par o) est dependant de son fichier correspondant 
					# finisant par c et de libft.h 
					# $< se lit : premier prerequis (1er dependance) donc le fichier .c
					# $@ se lit : la target donc le fichier .o qu on veut creer
					# pourquoi libft.h ? parce que ainsi chaque fichier .o depend de son fichier source .c ET de son header (.h) 
					# donc si le fichle fichier .h est modifie le make saura quil faut recompiler

clean:
	rm -f $(OBJS)			# supprime tout les fichier objets 
					# -f : force = sans demander confirmation
fclean:
	rm -f $(NAME)			# supprime librairie statique (fichiers pre-compile)

re: fclean all 				# nettoie tout puis recompile

.PHONY: all clean fclean re 		# evite les confilts avec les fichiers du meme nom 
