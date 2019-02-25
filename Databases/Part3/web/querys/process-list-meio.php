<html>
    <body>
<?php
    $process = $_REQUEST['process'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
    
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
        $sql = "SELECT num_meio, nome_meio, nome_entidade FROM acciona NATURAL JOIN meio WHERE num_processo_socorro = :process;";
    
        $result = $db->prepare($sql);
        $result->execute([':process' => $process]);
    
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
        echo("</table>\n");
    
        $db = null;

        echo "<br>Operation Successful<br>";
        echo "<a href='../index.html'>Voltar ao inicio</a>";
    }
    catch (PDOException $e)
    {
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
?>
    </body>
</html>
