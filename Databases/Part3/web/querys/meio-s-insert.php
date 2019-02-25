<html>
    <body>
<?php
    $numero = $_REQUEST['numero'];
    $nome = $_REQUEST['nome'];
    $entidade = $_REQUEST['entidade'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "INSERT INTO meio (num_meio, nome_meio, nome_entidade) VALUES (:numero, :nome, :entidade);";
        $result = $db->prepare($sql);
        $result->execute([':numero' => $numero, ':nome' => $nome, ':entidade' => $entidade]);

        $sql = "INSERT INTO meiosocorro (num_meio, nome_entidade) VALUES (:numero, :entidade);";
        $result = $db->prepare($sql);
        $result->execute([':numero' => $numero, ':entidade' => $entidade]);
        
        $db = null;

        echo "Operation Successful<br>";
        echo "<a href='../index.html'>Voltar ao inicio</a>";
    }
    catch (PDOException $e)
    {
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
?>
    </body>
</html>
