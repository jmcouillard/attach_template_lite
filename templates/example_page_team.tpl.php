<div class="container">

	<div class="row">

	<?php $i=0; foreach($data["team"] as $reseller) { ?>

		<?php if($i!=0 && $i%3==0) { ?>
			</div> <!-- end .row -->
			<div class="row"> <!-- begin .row -->
		<?php } ?>

		<div class="four columns alpha <?= ($i%3!=0) ? "offset-by-two" : "" ?>">
			<?= drupal_render($reseller); ?>
		</div>

	<?php $i++; } ?>

	</div> <!-- end .row -->

</div> <!-- end .container -->


