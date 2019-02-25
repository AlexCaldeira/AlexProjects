<html>
    <body>
<?php
    $telefone = $_REQUEST['telefone'];
    $instante = $_REQUEST['instante'];
    $nome = $_REQUEST['nome'];
    $local = $_REQUEST['local'];
    $processo = $_REQUEST['processo'];

    if(empty($processo)){
        $processo = null;
    }

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "INSERT INTO eventoemergencia (num_telefone, instante_chamada, nome_pessoa, morada, num_processo_socorro)
                VALUES (:telefone, :instante, :nome, :local, :processo);";

        $result = $db->prepare($sql);
        $result->execute([':telefone' => $telefone, ':instante' => $instante, ':nome' => $nome, ':local' => $local, ':processo' => $processo]);
        
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