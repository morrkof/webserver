<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <link href="styles/site.css" rel="stylesheet">
    <script src="scripts/jquery.js"></script>
    <script src="scripts/site.js"></script>
    <title>Онлайн магазин грибов</title>
</head>
<body>
<header>
    <div id="headerInside">
        <div id="logo"></div>
        <div id="companyName">Troglodits</div>
        <div id="navWrap">
            <a href="/">
                Главная
            </a>
            <a href="index.php?page=shop">
                Магазин
            </a>
        </div>
    </div>
</header>

<div id="content">

    <?php
    $goods = [
        [
            'id' => 1,
            'name' => 'Мухомор',
            'desc' => 'Мухомор - это не только ценный мех, но еще и 3 килограмма тяжелой, плохо перевариваемой пищи.',
            'img' => '/images/goods/amanita.jpg',
            'price' => '2000 💰'
        ],
        [
            'id' => 2,
            'name' => 'Светящаяся сыроежка',
            'desc' => 'Светящаяся сыроежка - самое редкое грибное животное, оно обитает в тропиках и питается светящимися грибами.',
            'img' => '/images/goods/russula.jpg',
            'price' => '1200 💰'
        ],
        [
            'id' => 3,
            'name' => 'Лиловый копринус',
            'desc' => 'Лиловый копринус - это очень вкусный гриб, он не ядовитый, но может испортить любой праздничный стол!',
            'img' => '/images/goods/coprinus.png',
            'price' => '1400 💰'
        ],
    ];

    $page = $_GET['page'];

    if (!isset($page)) {
        require('templates/main.php');
    } elseif ($page == 'shop') {
        require('templates/shop.php');
    } elseif ($page == 'product') {
        $id = $_GET['id'];
        $good = [];
        foreach ($goods as $product) {
            if ($product['id'] == $id) {
                $good = $product;
                break;
            }
        }
        require('templates/openedProduct.php');
    }
?>

</div>

<footer>
    <div id="footerInside">

        <div id="contacts">
            <div class="contactWrap">
                <img src="images/envelope.svg" class="contactIcon">
                info@troglodits.ru
            </div>
            <div class="contactWrap">
                <img src="images/phone-call.svg" class="contactIcon">
                8 800 555 35 35
            </div>
            <div class="contactWrap">
                <img src="images/placeholder.svg" class="contactIcon">
                Самая дальняя пещера
            </div>
        </div>

        <div id="footerNav">
            <a href="/">Главная</a>
            <a href="index.php?page=shop">Магазин</a>
        </div>

        <div id="social">
            <img class="socialItem" src="images/vk-social-logotype.svg">
            <img class="socialItem" src="images/google-plus-social-logotype.svg">
            <img class="socialItem" src="images/facebook-logo.svg">
        </div>

        <div id="copyrights">&copy; Troglodits</div>
    </div>
</footer>

</body>
</html>