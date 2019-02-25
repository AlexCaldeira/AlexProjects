<html>
    <body>
<?php
    $local = $_REQUEST['local'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="ist187665";
        $password = "zofx9020";
        $dbname = $user;
    
        $db = new PDO("pgsql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    
        $sql = "SELECT num_meio, nome_meio, nome_entidade
                FROM eventoemergencia e NATURAL JOIN acciona a NATURAL JOIN meio m
                WHERE morada = :local
                AND EXISTS(
                    SELECT num_meio, nome_entidade
                    FROM meiosocorro s
                    WHERE a.num_meio = s.num_meio
                    AND a.nome_entidade = s.nome_entidade
                );";
    
        $result = $db->prepare($sql);
        $result->execute([':local' => $local]);
    
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
