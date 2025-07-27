SELECT l.titulo, au.nome_autor
FROM Livros l
JOIN Livro_Autor la ON l.codigo_livro = la.fk_livro
JOIN Autores au ON la.fk_autor = au.id_autor
ORDER BY l.titulo;