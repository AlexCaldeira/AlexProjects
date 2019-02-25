<html>
    <body>
<?php
    $telefone = $_REQUEST['telefone'];
    $instante = $_REQUEST['instante'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "DELETE FROM eventoemergencia
                WHERE num_telefone = :telefone
                AND instante_chamada = :instante;";

        $result = $db->prepare($sql);
        $result->execute([':telefone' => $telefone, ':instante' => $instante]);
        
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