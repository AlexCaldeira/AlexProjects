<html>

	<head>
  		<meta charset="UTF-8">
	</head>

    <body>
<?php
   $query = $_REQUEST['arg'];

	try
	{
		$host = "db.ist.utl.pt";
		$user ="ist187665";
		$password = "zofx9020";
		$dbname = $user;

		$db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
		$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	   switch ($query) {
	   	case "local-in":
	   		echo "<h2>Inserir local</h2>";
				echo "<form action ='querys/local-insert.php' method='POST'>";
				echo "<p>Qual a morada?: <input type='text' name='local'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "local-rm":
	   		echo "<h2>Apagar local</h2>";
				echo "<form action ='querys/local-delete.php' method='POST'>";
				echo "<p>Qual a morada?: <input type='text' name='local'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "emergency-in":
	   		echo "<h2>Inserir Emergência</h2>";
				echo "<form action ='querys/emergency-insert.php' method='POST'>";
				echo "<p>Qual o telefone?: <input type='text' name='telefone'/></p>";
				echo "<p>Qual o instante da chamada?: <input type='text' name='instante'/></p>";
				echo "<p>Qual o nome da pessoa?: <input type='text' name='nome'/></p>";
				echo "<p>Qual a morada?: <input type='text' name='local'/></p>";
				echo "<p>Qual o número do processo? (opcional): <input type='text' name='processo'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "emergency-rm":
	   		echo "<h2>Apagar Emergência</h2>";
				echo "<form action ='querys/emergency-delete.php' method='POST'>";
				echo "<p>Qual o número de telefone?: <input type='text' name='telefone'/></p>";
				echo "<p>Qual o instante da chamada?: <input type='text' name='instante'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-in":
	   		echo "<h2>Inserir processo de socorro</h2>";
				echo "<form action ='querys/process-insert.php' method='POST'>";
				echo "<p>Qual o número do processo de socorro?: <input type='text' name='process'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-rm":
	   		echo "<h2>Apagar processo de socorro</h2>";
				echo "<form action ='querys/process-remove.php' method='POST'>";
				echo "<p>Qual o número do processo de socorro?: <input type='text' name='process'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-ls":
				$sql = "SELECT num_processo_socorro FROM processosocorro;";
				$result = $db->prepare($sql);
				$result->execute();

				echo("<table border=\"1\">\n");
				echo("<tr><td>num_processo_socorro</td></tr>\n");
				foreach($result as $row)
				{
					echo("<tr><td>");
					echo($row['num_processo_socorro']);
					echo("</td></tr>\n");
				}
				echo("</table>\n\n");

				echo "<a href='index.html'>Voltar ao inicio</a>";
	   		break;

	   	case "proc-meio":
	   		echo "<h2>Associar processo a um meio</h2>";
				echo "<form action ='querys/proc-meio-assoc.php' method='POST'>";
				echo "<h4>Processo</h4>";
				echo "<p>Qual o número do processo?: <input type='text' name='process'/></p>";
				echo "<h4>Meio</h4>";
				echo "<p>Qual o número do meio?: <input type='text' name='num_meio'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='ent_meio'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-event":
	   		echo "<h2>Associar processo a um evento de emergência</h2>";
				echo "<form action ='querys/proc-emergency-assoc.php' method='POST'>";
				echo "<h3>Processo</h3>";
				echo "<p>Qual o número do processo a associar?: <input type='text' name='processo'/></p>";
				echo "<h4>Evento de Emergência</h4>";
				echo "<p>Qual o telefone?: <input type='text' name='telefone'/></p>";
				echo "<p>Qual o instante da chamada?: <input type='text' name='instante'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-ls-meio":
	   		echo "<h2>Listar meios acionados num processo de socorro</h2>";
				echo "<form action ='querys/process-list-meio.php' method='POST'>";
				echo "<p>Qual o número do processo de socorro?: <input type='text' name='process'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "proc-ls-local":
	   		echo "<h2>Listar meios de socorro acionados num processo de socorro dado num certo local</h2>";
				echo "<form action ='querys/process-list-local.php' method='POST'>";
				echo "<p>Qual o local do incêndio?: <input type='text' name='local'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-in":
	   		echo "<h2>Inserir meios</h2>";
				echo "<form action ='querys/meio-insert.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-rm":
	   		echo "<h2>Apagar Meio</h2>";
				echo "<form action ='querys/meio-delete.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-ls":
	   		$sql = "SELECT num_meio, nome_meio, nome_entidade FROM meio;";
				$result = $db->prepare($sql);
				$result->execute();

				echo("<table border=\"1\">\n");
				echo("<tr><td>num_meio</td><td>nome_meio</td><td>nome_entidade</td></tr>\n");
				foreach($result as $row)
				{
					echo("<tr><td>");
					echo($row['num_meio']);
					echo("</td><td>");
					echo($row['nome_meio']);
					echo("</td><td>");
					echo($row['nome_entidade']);
					echo("</td></tr>\n");
				}
				echo("</table>\n\n");

				echo "<a href='index.html'>Voltar ao inicio</a>";
	   		break;

	   	case "ent-in":
	   		echo "<h2>Inserir entidade</h2>";
				echo "<form action ='querys/entity-insert.php' method='POST'>";
				echo "<p>Qual o nome da entidade?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";break;

	   	case "ent-rm":
	   		echo "<h2>Apagar entidade</h2>";
				echo "<form action ='querys/entity-delete.php' method='POST'>";
				echo "<p>Qual o nome da entidade?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-c-in":
	   		echo "<h2>Inserir meio de combate</h2>";
				echo "<form action ='querys/meio-c-insert.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-c-rm":
	   		echo "<h2>Apagar meio de combate</h2>";
				echo "<form action ='querys/meio-c-remove.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-c-ed":
	   		echo "<h2>Editar meio de combate</h2>";
				echo "<form action ='querys/meio-c-edit.php' method='POST'>";
				echo "<h3>Qual o meio que quer editar?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero-old'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade-old'/></p>";
				echo "<h3>Quais são os novos detalhes do meio?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-a-in":
	   		echo "<h2>Inserir meio de apoio</h2>";
				echo "<form action ='querys/meio-a-insert.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-a-rm":
	   		echo "<h2>Apagar meio de apoio</h2>";
				echo "<form action ='querys/meio-a-remove.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-a-ed":
	   		echo "<h2>Editar meio de apoio</h2>";
				echo "<form action ='querys/meio-a-edit.php' method='POST'>";
				echo "<h3>Qual o meio que quer editar?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero-old'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade-old'/></p>";
				echo "<h3>Quais são os novos detalhes do meio?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-s-in":
	   		echo "<h2>Inserir meio de socorro</h2>";
				echo "<form action ='querys/meio-s-insert.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-s-rm":
	   		echo "<h2>Apagar meio de socorro</h2>";
				echo "<form action ='querys/meio-s-remove.php' method='POST'>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;

	   	case "meio-s-ed":
	   		echo "<h2>Editar meio de socorro</h2>";
				echo "<form action ='querys/meio-s-edit.php' method='POST'>";
				echo "<h3>Qual o meio que quer editar?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero-old'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade-old'/></p>";
				echo "<h3>Quais são os novos detalhes do meio?</h3>";
				echo "<p>Qual o número do meio?: <input type='text' name='numero'/></p>";
				echo "<p>Qual o nome do meio?: <input type='text' name='nome'/></p>";
				echo "<p>Qual o nome da entidade do meio?: <input type='text' name='entidade'/></p>";
				echo "<p><input type='submit' value='Submit'/></p>";
				echo "</form>";
	   		break;
	   	
	   	default:
	   		echo "The requested operation could not be parsed";
	   		break;
	   }

		$db = null;
	}

	catch (PDOException $e)
	{
		echo("<p>ERROR: {$e->getMessage()}</p>");
	}

?>
    </body>
</html>
