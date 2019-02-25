<html>
    <body>
<?php
    $numero_old = $_REQUEST['numero-old'];
    $entidade_old = $_REQUEST['entidade-old'];
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

        $sql = "UPDATE meio
                SET num_meio = :numero
                , nome_meio = :nome
                , nome_entidade = :entidade
                WHERE num_meio = :numero_old
                AND nome_entidade = :entidade_old;";

        $result = $db->prepare($sql);
        $result->execute([':numero' => $numero, ':nome' => $nome, ':entidade' => $entidade, ':numero_old' => $numero_old, ':entidade_old' => $entidade_old]);
        
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
