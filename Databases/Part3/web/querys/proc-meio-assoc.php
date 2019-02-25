<html>
    <body>
<?php
    $process = $_REQUEST['process'];
    $num_meio = $_REQUEST['num_meio'];
    $ent_meio = $_REQUEST['ent_meio'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $sql = "INSERT INTO acciona (num_meio, nome_entidade, num_processo_socorro) VALUES (:num_meio, :ent_meio, :process);";

        $result = $db->prepare($sql);
        $result->execute([':process' => $process, ':num_meio' => $num_meio, ':ent_meio' => $ent_meio]);
        
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
