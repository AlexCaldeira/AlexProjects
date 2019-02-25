<html>
    <body>
<?php
    $processo = $_REQUEST['processo'];
    $telefone = "'". $_REQUEST['telefone']. "'";
    $instante = "'". $_REQUEST['instante']. "'";

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $query = "UPDATE eventoemergencia
                SET num_processo_socorro = :processo
                WHERE num_telefone = :telefone
                AND instante_chamada = :instante;";

        $result = $db->prepare($query);
        $result->execute([':processo' => $processo, ':telefone' => $telefone, ':instante' => $instante]);
        
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